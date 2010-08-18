#ifndef FSM_H
#define FSM_H

int xdcc_process(char *string, int sockfd);
int init_processor(char *path);

struct file_data {
    char *filename;
    char *absolute_path;
    char *filedata;
    struct stat *info;
};

#endif /* FSM_H */
