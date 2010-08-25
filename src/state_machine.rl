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
        if (botname && bstrcmp(botname, bot_nickname) != 0)
            fbreak;
    }

    action packnumber {
        digits = blk2bstr(mark, fpc-mark);
        irc_req->number = atoi(bdata(digits));
        bdestroy(digits);
    }

    action remote_nick {
        irc_req->remote_nick = blk2bstr(mark, fpc-mark);
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

irc_request *irc_parser(bstring buffer)
{
    if (buffer == NULL)
        return NULL;

    int cs = 0;
    const char *p = bdata(buffer);
    const char *pe = p+blength(buffer);
    const char *eof = NULL;
    const char *mark = p;
    bstring botname = NULL, digits = NULL;
    irc_request *irc_req = IrcRequest_create();

    %% write init;
    %% write exec;

    bdestroy(botname);

    if ( cs < %%{ write first_final; }%% ) {
        irc_req->op = INVALID;
        return irc_req;
    }

    return irc_req;
}

