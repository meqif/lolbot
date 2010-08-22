/* See LICENSE file for copyright and license details. */

#ifndef handler_h
#define handler_h

#include "state_machine.h"

struct file_data {
    char *filename;
    char *absolute_path;
    char *desc;
    unsigned long size;
};

int handler(int sockfd, irc_request *irc_req);
int init_server(char *path);
int tear_down_server();

#endif
