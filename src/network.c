#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>

#define BUFSIZE 1024

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

char *get_external_ip()
{
    char *ip, *buf = malloc(BUFSIZE * sizeof(char));
    int sockfd;
    FILE *interwebs;

    sockfd = create_socket("whatismyip.org", "80");
    send(sockfd, "GET / HTTP/1.0\r\n\r\n", 18, 0);

    interwebs = fdopen(sockfd, "r");
    while(!feof(interwebs)) {
        memset(buf, 0, BUFSIZE * sizeof(char));
        fgets(buf, BUFSIZE, interwebs);
    }

    ip = calloc(strlen(buf)+1, sizeof(char));
    strcpy(ip, buf);

#ifdef DEBUG
    printf("Got external IP: %s\n", ip);
#endif

    free(buf);
    fclose(interwebs);
    close(sockfd);

    return ip;
}

// vim: et ts=4 sw=4 sts=4
