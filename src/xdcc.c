#include "network.h"
#include "irc.h"
#include "state_machine.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <errno.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define BUF_SIZE 1000
#define FILE_BUFSIZE 10240
#define BOTNAME loldrop

char *shared_path, *ip;
int nfiles;
struct file_data *files;

/* Converts an IPv4 address given in a string to the corresponding integer */
int foo(char *str)
{
    union {
        int x;
        unsigned char y[4];
    } gamma;

    char *ptr = str;
    int i = 0;

    while(i < 4) {
        if (*str == '.' || *str == 0) {
            char *tmp = calloc(4, sizeof(char));
            strncpy(tmp, ptr, str-ptr);
            gamma.y[i++] = atoi(tmp);
            free(tmp);
            ptr = str+1;
        }
        str++;
    }

    return gamma.x;
}

static
off_t fsize(char *filename)
{
    struct stat *buf = malloc(sizeof (struct stat));
    stat(filename, buf);
    return buf->st_size;
}

int xdcc_list(char *remote_nick, int sockfd)
{
    int i;
    for (i = 0; i < nfiles; i++) {
        irc_privmsg(sockfd, remote_nick, files[i].filedata);
    }

    return 0;
}

static
char *xdcc_info(char *string)
{
    return NULL;
}

int xdcc_send(struct file_data requested_file, char *remote_nick, int sockfd)
{
    char *port = "8888";
    int porti = atoi(port);

    // Create listening socket
    int newsock = create_socket(NULL, port);
    listen(newsock, 4);

    // Send details to client
    int addr = htonl(foo(ip));

    unsigned long filesize = fsize(requested_file.absolute_path);
    irc_dcc_send(sockfd, remote_nick, requested_file.filename, filesize, addr, porti);

    // Send/Resume file
    struct sockaddr their_addr;
    socklen_t addr_size;
    int sock = accept(newsock, &their_addr, &addr_size);

    FILE *file = fopen(requested_file.absolute_path, "r");
    unsigned char *buffer = malloc(FILE_BUFSIZE);
    char ack[4];

    while (!feof(file)) {
        memset(buffer, FILE_BUFSIZE, sizeof(char));

        // Send block
        int len = fread(buffer, sizeof(char), FILE_BUFSIZE, file);
        send(sock, buffer, len, 0);

        // Receive 4-byte ACK
        recv(sock, ack, 4*sizeof(char), 0);
    }

    close(sock);
    close(newsock);

    return 0;
}

static
void xdcc_remove(int position)
{
    if (position == -1)
        return;
}
