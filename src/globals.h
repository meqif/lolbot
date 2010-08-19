/* See LICENSE file for copyright and license details. */

#ifndef globals_h
#define globals_h

enum irc_operation {
	INVALID = -1,
    SEND,
    INFO,
    LIST,
    REMOVE,
    RESUME,
    PING,
    QUIT
};

#endif
