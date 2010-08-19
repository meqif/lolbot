#include "handler.h"
#include "network.h"
#include "irc.h"
#include "xdcc.h"
#include "globals.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <errno.h>
#include <unistd.h>

#define BUF_SIZE 1000

char *shared_path, *ip;
int nfiles;
struct file_data *files;

/* String comparison for quicksort */
static
int string_cmp(const void *a, const void *b)
{
    const char **ia = (const char **)a;
    const char **ib = (const char **)b;
    return strcmp(*ia, *ib);
}

int handler(int sockfd, struct irc_request *irc_req)
{
    switch(irc_req->op) {
        case QUIT:
            /* Sir, you shouldn't be here. */
            return 1;
        case PING:
            irc_pong(sockfd, irc_req->remote_nick);
            break;
        case LIST:
            xdcc_list(irc_req->remote_nick, sockfd);
            break;
        case SEND:
            if (irc_req->number > 0)
                xdcc_send(&files[irc_req->number-1], irc_req->remote_nick, sockfd);
            break;
        default:
            break;
    }

    return 0;
}

int init_server(char *path)
{
    struct dirent *d;
    struct stat *s;
    DIR *dir;

    ip = get_external_ip();
    shared_path = path;

    /* Initialize file list cache */
    dir = opendir(path);

    if (dir != NULL) {
        int i = 0, dir_size = 0;

        /* Count used inodes, so we can create an array of the correct size */
        while ( (d = readdir(dir)) )
            if (d->d_ino != 0) dir_size++;

        closedir(dir);
        dir = opendir(path);

        files = malloc(dir_size * sizeof(struct file_data));
        char *list[dir_size];
        while ( (d = readdir(dir)) ) {
            if (d->d_ino != 0) /* Ignore invalid inodes */
                list[i++] = strdup(d->d_name);
        }
        list[i] = NULL;

        /* Sort entries */
        qsort(list, dir_size, sizeof(char *), string_cmp);

        int idx;
        for (idx = 0, i = 0; i < dir_size; i++) {
            char *name = list[i];
            if (strncmp(name, ".", 1) != 0) {  /* Don't list hidden files and directories */
                char *buf = calloc(strlen(path) + strlen(name) + 2, sizeof(char));
                sprintf(buf, "%s/%s", path, name);
#ifdef DEBUG
                fprintf(stderr, "%s\n", buf);
#endif
                char *start = calloc(strlen(buf)+100, sizeof(char));
                s = malloc(sizeof(struct stat));
                lstat(buf, s);
                unsigned long size = s->st_size; /* Using long for supporting files >=4GiB */
                unsigned int sizeMB = size/(1024*1024);
                sprintf(start, "#%d [%uMB] %s\n", idx+1, sizeMB, name);
                files[idx].filename = strdup(name);
                files[idx].absolute_path = buf;
                files[idx].filedata = strdup(start);
                files[idx].info = s;
                free(start);
                idx++;
            }
            free(list[i]);
        }
        nfiles = idx;

        closedir(dir);
    }
   else {
        perror(path);
        return 1;
    }

    return 0;
}
