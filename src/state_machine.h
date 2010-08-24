#ifndef FSM_H
#define FSM_H

#include "globals.h"
#include "irc_request.h"

irc_request *irc_parser(char *string);

#endif /* FSM_H */
