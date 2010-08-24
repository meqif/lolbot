#include "globals.h"
#include "irc_request.h"
#include <stdlib.h>

void IrcRequest_free(irc_request *irc_req)
{
    if (irc_req == NULL)
        return;

    if (irc_req != NULL)
        free(irc_req->remote_nick);

    free(irc_req);
}

// vim: et ts=4 sw=4 sts=4
