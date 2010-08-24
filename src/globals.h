/* See LICENSE file for copyright and license details. */

#ifndef globals_h
#define globals_h

#include <sys/socket.h>

/* Compatibility for MSG_NOSIGNAL-deprived OSes, like OSX. */
#ifndef MSG_NOSIGNAL
#define MSG_NOSIGNAL 0
#endif

#endif
