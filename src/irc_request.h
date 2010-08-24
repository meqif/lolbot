/* See LICENSE file for copyright and license details. */

#ifndef irc_request_h
#define irc_request_h

enum irc_operation {
    INVALID,
    SEND,
    INFO,
    LIST,
    REMOVE,
    RESUME,
    PING,
    QUIT
};

typedef struct {
    char *remote_nick;
    enum irc_operation op;
    int number;
} irc_request;

#endif
