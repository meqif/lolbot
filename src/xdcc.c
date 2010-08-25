#include "xdcc.h"
#include "network.h"
#include "irc.h"
#include "handler.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define BUF_SIZE 1000
#define FILE_BUFSIZE 1024*10

extern char *ip;
extern int nfiles;
extern struct file_data *files;
struct xdcc_request *request;

int xdcc_list(char *remote_nick, int sockfd)
{
    int i;
    for (i = 0; i < nfiles; i++) {
        irc_notice(sockfd, remote_nick, files[i].desc);
    }

    return 0;
}

int xdcc_send(struct file_data *requested_file, char *remote_nick, int sockfd)
{
    char *port = "8888";
    int porti = atoi(port);
    int addr;
    struct in_addr addr_;

    if (inet_aton(ip, &addr_) == 0) {
        perror("inet_aton");
        return 1;
    } else
        addr = addr_.s_addr;

    // Create listening socket
    int newsock = create_socket(NULL, port);
    listen(newsock, 4);

    // Send details to client
    request = malloc(sizeof(struct xdcc_request));
    request->file = requested_file;
    request->offset = 0;
    request->nick = remote_nick;

    irc_dcc_send(sockfd, request->nick, request->file->filename, request->file->size, addr, porti);

    struct timeval tv, tv_original;
    socklen_t tv_length = sizeof(struct timeval);
    tv.tv_sec = 5;
    tv.tv_usec = 0;

    // Save socket timeout settings
    getsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv_original, &tv_length);

    setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (char *)&tv, sizeof(struct timeval));

    char *buf = calloc(1024, sizeof(char));
    int err = recv(sockfd, buf, 1024, MSG_PEEK|MSG_NOSIGNAL);

    if (err == ETIMEDOUT)
        fprintf(stderr, "Resume request timed out, proceeding...\n");
    else {
        if (strlen(buf) > 0 && strstr(buf, "RESUME")) {
            request->offset = atoi(strrchr(buf, ' '));
            irc_dcc_accept(sockfd, request->nick, request->file->filename, porti, request->offset);
        }
    }

    // Restore original timeout settings
    setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv_original, sizeof(struct timeval));

    free(buf);

#ifndef NDEBUG
    printf("Offset: %lu\n", request->offset);
#endif

    // Send/Resume file
    struct sockaddr *their_addr = calloc(1, sizeof(struct sockaddr));
    socklen_t addr_size;
    int sock = accept(newsock, their_addr, &addr_size);

#ifdef SO_NOSIGPIPE
    int set = 1;
    setsockopt(sock, SOL_SOCKET, SO_NOSIGPIPE, (void *)&set, sizeof(int));
#endif

    FILE *file = fopen(request->file->absolute_path, "r");
    unsigned char *buffer = malloc(FILE_BUFSIZE);
    unsigned int ack = 0;

#ifndef NDEBUG
    printf("Offset: %lu\n", request->offset);
#endif

    if (request->offset > 0)
        if (fseek(file, request->offset, SEEK_SET))
            perror("fseek");

    unsigned int nsent = request->offset;
    while (!feof(file)) {
        memset(buffer, FILE_BUFSIZE, sizeof(char));

        // Send block
        int len = fread(buffer, sizeof(char), FILE_BUFSIZE, file);
        if (send(sock, buffer, len, MSG_NOSIGNAL) == -1)
            break;

        nsent += len;

        // Receive 4-byte ACK
        err = 0;
        while (ntohl(ack) < nsent) {
            if ((err = recv(sock, &ack, 4, MSG_NOSIGNAL)) == -1) {
                break;
            }
        }
        //printf("Sent %d bytes, at %u bytes, receiver got %u bytes\n", len, nsent, ntohl(ack));

        if (err == -1)
            break;
    }

    free(their_addr);
    free(buffer);
    fclose(file);
    close(sock);
    close(newsock);

    return 0;
}

