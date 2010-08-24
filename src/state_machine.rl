#include "state_machine.h"
#include "globals.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <bstring.h>

extern bstring bot_nickname;

%%{
    machine irc_parser;

    action mark { mark = fpc; }

    action botname {
        botname = blk2bstr(mark, fpc-mark);
    }

    action packnumber {
        digits = blk2bstr(mark, fpc-mark);
    }

    action remote_nick {
        remote_nick = blk2bstr(mark, fpc-mark);
    }

    botname = alnum+ >mark %botname;
    address = "~" (alnum|"@"|"-"|".")+;
    whitespace = space+;
    multi =
        (
            "send"   @{ irc_req->op = SEND; } |
            "info"   @{ irc_req->op = INFO; } |
            "remove" @{ irc_req->op = REMOVE; }
        ) whitespace "#"? (digit+ >mark %packnumber);

    single =
        (
            "list"   @{ irc_req->op = LIST; } |
            "remove" @{ irc_req->op = REMOVE; }
        );

    filler = ":" alnum+ >mark %remote_nick "!" address " PRIVMSG " botname whitespace ":" whitespace*;
    xdcc = "xdcc" whitespace (single|multi);
    ping = ("PING" @{ irc_req->op = PING; }) whitespace ":"? ((alnum|".")+ >mark %remote_nick);
    admin = "admin" whitespace "0x123456789" whitespace "quit" @{ irc_req->op = QUIT; };

    main :=
            ( filler xdcc whitespace* ) |
            ( filler admin whitespace*) |
            ping whitespace*;
}%%

%% write data;

irc_request *irc_parser(char *string)
{
    if (string == NULL)
        return NULL;

    int cs = 0;
    int len = strlen(string);
    const char *p = string;
    const char *pe = p+len;
    const char *eof = NULL;
    const char *mark = p;
    bstring botname = NULL, digits = NULL, remote_nick;
    irc_request *irc_req = calloc(1, sizeof(irc_request));
    irc_req->op = INVALID;

    %% write init;

    %% write exec;

    if (botname && bstrcmp(botname, bot_nickname) != 0) {
        bdestroy(botname);
        irc_req->op = INVALID;
        return irc_req;
    }
    bdestroy(botname);

    if ( cs < %%{ write first_final; }%% ) {
        irc_req->op = INVALID;
        return irc_req;
    }

    irc_req->remote_nick = bdata(remote_nick);

    if (digits) {
        irc_req->number = atoi(bdata(digits));
        bdestroy(digits);
    } else
        irc_req->number = -1;

    return irc_req;
}

