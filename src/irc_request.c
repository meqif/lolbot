#include "globals.h"
#include "irc_request.h"
#include <stdlib.h>

irc_request *IrcRequest_create()
{
    irc_request *irc_req = calloc(sizeof(irc_request), 1);

    irc_req->op = INVALID;
    irc_req->number = -1;

    return irc_req;
}

void IrcRequest_free(irc_request *irc_req)
{
    if (irc_req == NULL)
        return;

    if (irc_req != NULL)
        bdestroy(irc_req->remote_nick);

    free(irc_req);
}

// vim: et ts=4 sw=4 sts=4
