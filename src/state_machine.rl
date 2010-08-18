#include "state_machine.h"
#include "network.h"
#include "irc.h"
#include "xdcc.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <errno.h>
#include <unistd.h>

#define BUF_SIZE 1000
#define FILE_BUFSIZE 10240

char *shared_path, *ip;
int nfiles;
struct file_data *files;

static
enum operation {
    SEND,
    INFO,
    LIST,
    REMOVE,
    PING
};

%%{
    machine irc;

    address = "~" (alnum|"@"|"-"|".")+;
    botname = "loldrop";

    whitespace = space+;
    multi =
        (
            "send"   @{ op = SEND; } |
            "info"   @{ op = INFO; } |
            "remove" @{ op = REMOVE; }
        ) whitespace "#"? (digit+ >{ digit_start = p; } @{ digit_end = p+1; } );

    single =
        (
            "list"   @{ op = LIST; } |
            "remove" @{ op = REMOVE; }
        );

    filler = ":" alnum+ ("!" >{ nick_size = p-nick_start; }) address " PRIVMSG " botname whitespace ":" whitespace*;

    xdcc = ("xdcc" >{ command = p; }) whitespace (single|multi);
    ping = ("PING" @{ op = PING; }) whitespace ":"? ((alnum|".")+ >{nick_start = p, nick_size = 0;} @{nick_size++;});

    main :=
            ( filler xdcc whitespace* ) |
            ping whitespace*;
}%%

%% write data;

/* String comparison for quicksort */
static
int string_cmp(const void *a, const void *b)
{
    const char **ia = (const char **)a;
    const char **ib = (const char **)b;
    return strcmp(*ia, *ib);
}

static
int _xdcc_process(char *string, int len, int sockfd)
{
    int cs, desired_file;
    char *p = string, *pe, *remote_nick, *nick_start, *command;
    char *digit_start = NULL, *digit_end = NULL, *digits;
    size_t nick_size, s;
    enum operation op = 0;

    nick_start = string+1;

    %% write init;

    pe = p + len;

    %% write exec;

    if ( cs < %%{ write first_final; }%% ) {
        return 1;
    }

    remote_nick = calloc(nick_size+1, sizeof (char));
    strncpy(remote_nick, nick_start, nick_size);
#ifdef DEBUG
    printf("Remote nick: %s\n", remote_nick);
    printf("Command: %s\n", command);
#endif

    switch(op) {
        case PING:
            irc_pong(sockfd, remote_nick);
            break;
        case LIST:
            xdcc_list(remote_nick, sockfd);
            break;
        case SEND:
            if (digit_start && digit_end) {
                s = digit_end - digit_start;
                digits = calloc(s+1, sizeof(char));
                strncpy(digits, digit_start, s);
                desired_file = atoi(digits)-1;
            }
            if (desired_file >= 0)
                xdcc_send(files[desired_file], remote_nick, sockfd);
            break;
        default:
            irc_privmsg(sockfd, remote_nick, command);
            break;
    }

    return 0;
}

int xdcc_process(char *string, int sockfd)
{
    int len = strlen(string);
    return _xdcc_process(string, len, sockfd);
}

int init_processor(char *path)
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
                fprintf(stderr, "%s\n", buf);
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
