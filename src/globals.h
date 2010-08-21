/* See LICENSE file for copyright and license details. */

#ifndef globals_h
#define globals_h

#ifndef MSG_NOSIGNAL
#define MSG_NOSIGNAL 0
#endif

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
