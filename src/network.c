#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>

void send_message(char *remote_nick, char *type, char *message, int sockfd)
{
    size_t s = strlen(message) + strlen(type) + strlen(remote_nick) + 5 + 1;
    char *cmd = calloc(s, sizeof (char));
    snprintf(cmd, s, "%s %s :%s\r\n", type, remote_nick, message);
#ifdef DEBUG
    printf("%s\n", cmd);
#endif
    send(sockfd, cmd, strlen(cmd), 0);
}

int create_socket(char *hostname, char *port)
{
    int sockfd, err;
    struct addrinfo hints, *res;

    /* Hints about the type of socket */
    memset(&hints, 0, sizeof hints);
    hints.ai_family   = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if (hostname == NULL) {
       hints.ai_flags = AI_PASSIVE;
    }

    /* Translate address and service information */
    err = getaddrinfo(hostname, port, &hints, &res);
    if (err || res == NULL) {
        fprintf(stderr, "Failed to lookup %s\n", hostname);
        exit(EXIT_FAILURE);
    }

    /* Create socket */
    sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (sockfd == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    if (hostname != NULL) {
        /* Initiate connection */
        err = connect(sockfd, res->ai_addr, res->ai_addrlen);
        if (err == -1) {
            perror("connect");
            freeaddrinfo(res);
            exit(EXIT_FAILURE);
        }
    } else {
        err = bind(sockfd, res->ai_addr, res->ai_addrlen);
        if (err == -1) {
            perror("bind");
            freeaddrinfo(res);
            exit(EXIT_FAILURE);
        }
    }

    freeaddrinfo(res);

    return sockfd;
}

// vim: et ts=4 sw=4 sts=4
