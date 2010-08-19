#include "state_machine.h"
#include "globals.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

%%{
    machine irc_parser;

    address = "~" (alnum|"@"|"-"|".")+;
    botname = "loldrop";

    whitespace = space+;
    multi =
        (
            "send"   @{ op = SEND; } |
            "info"   @{ op = INFO; } |
            "remove" @{ op = REMOVE; }
        ) whitespace "#"? (digit+ >{ digit_start = p; } @{ digit_end = p+1; } );

    single =
        (
            "list"   @{ op = LIST; } |
            "remove" @{ op = REMOVE; }
        );

    filler = ":" alnum+ ("!" >{ nick_size = p-nick_start; }) address " PRIVMSG " botname whitespace ":" whitespace*;

    xdcc = ("xdcc" >{ command = p; }) whitespace (single|multi);
    ping = ("PING" @{ op = PING; }) whitespace ":"? ((alnum|".")+ >{nick_start = p, nick_size = 0;} @{nick_size++;});
    admin = "admin" whitespace "0x123456789" whitespace "quit" @{ op = QUIT; };

    main :=
            ( filler xdcc whitespace* ) |
            ( filler admin whitespace*) |
            ping whitespace*;
}%%

%% write data;

struct irc_request *irc_parser(char *string)
{
    int cs, len = strlen(string);
    char *p = string, *pe, *remote_nick, *nick_start, *command;
    char *digit_start = NULL, *digit_end = NULL, *digits;
    size_t nick_size, s;
    enum irc_operation op = INVALID;
    struct irc_request *irc_req = malloc(sizeof(struct irc_request));

    nick_start = string+1;

    %% write init;

    pe = p + len;

    %% write exec;

    if ( cs < %%{ write first_final; }%% ) {
        irc_req->op = INVALID;
        return irc_req;
    }

    remote_nick = calloc(nick_size+1, sizeof (char));
    strncpy(remote_nick, nick_start, nick_size);

    irc_req->remote_nick = remote_nick;
    irc_req->op = op;

    if (digit_start && digit_end) {
        s = digit_end - digit_start;
        digits = calloc(s+1, sizeof(char));
        strncpy(digits, digit_start, s);
        irc_req->number = atoi(digits);
    } else
        irc_req->number = -1;

#ifdef DEBUG
    printf("Remote nick: %s\n", remote_nick);
    printf("Command: %s\n", command);
#endif

    return irc_req;
}

