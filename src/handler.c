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
#include <bstring.h>

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
            if (irc_req->number > 0 && irc_req->number < nfiles)
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
                bstring absolute_path = bformat("%s/%s", path, name);
#ifdef DEBUG
                fprintf(stderr, "%s\n", bdata(absolute_path));
#endif
                s = malloc(sizeof(struct stat));
                lstat(bdata(absolute_path), s);
                unsigned long size = s->st_size; /* Using long for supporting files >=4GiB */
                unsigned int sizeMB = size/(1024*1024);
                bstring filedata = bformat("#%d [%uMB] %s\n", idx+1, sizeMB, name);
                files[idx].filename = strdup(name);
                files[idx].absolute_path = bstr2cstr(absolute_path, '\0');
                files[idx].filedata = bstr2cstr(filedata, '\0');
                files[idx].info = s;
                bdestroy(absolute_path);
                bdestroy(filedata);
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
