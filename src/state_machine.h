#ifndef FSM_H
#define FSM_H

#include "globals.h"

struct irc_request *xdcc_process(char *string);
int handler(int sockfd, struct irc_request *irc_req);
int init_processor(char *path);

struct file_data {
    char *filename;
    char *absolute_path;
    char *filedata;
    struct stat *info;
};

#endif /* FSM_H */
