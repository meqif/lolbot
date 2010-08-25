#ifndef FSM_H
#define FSM_H

#include "globals.h"
#include "irc_request.h"
#include <bstring.h>

irc_request *irc_parser(bstring buffer);

#endif /* FSM_H */
