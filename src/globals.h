/* See LICENSE file for copyright and license details. */

#ifndef globals_h
#define globals_h

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

struct irc_request {
	char *remote_nick;
	enum irc_operation op;
	int number;
};

#endif
