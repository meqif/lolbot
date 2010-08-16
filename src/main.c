#include "xdcc.h"
#include "network.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>

#define BUFSIZE 128
#define SERVER "irc.oftc.net"
#define PORT "6667"
#define SHARED_PATH "/Users/meqif/Downloads/[OMDA]_AZUMANGA_DAIOH_01-26/"

static void usage() {
    fprintf(stderr, "usage: loldrop [options]\n"
                    "  -s  --server\tirc server to connect\n"
                    "  -p  --port\tirc server's port (optional, defaults to 6667)\n"
                    "  -d  --path\tpath to shared directory\n");
    exit(1);
}

int main(int argc, const char *argv[])
{
    int sockfd, err;
    char *buf = malloc(BUFSIZE * sizeof(char));

    init_processor(SHARED_PATH);

    sockfd = create_socket(SERVER, PORT);

    FILE *interwebs = fdopen(sockfd, "r+");

    err = send(sockfd, "USER loldrop x x :loldrop\r\n", 27, 0);
    printf("%d\n", err);
    err = send(sockfd, "NICK loldrop\r\n", 14, 0);
    printf("%d\n", err);

    sleep(1);

    err = send(sockfd, "JOIN #loldrop\r\n", 15, 0);
    printf("%d\n", err);

    while (1) {
        memset(buf, 0, BUFSIZE);
        fgets(buf, BUFSIZE, interwebs);
        printf("%s", buf);
        xdcc_process(buf, sockfd);
    }

    err = send(sockfd, "QUIT\r\n", 6, 0);
    printf("%d\n", err);

    fclose(interwebs);
    close(sockfd);
    free(buf);

    return 0;
}

// vim: et ts=4 sw=4 sts=4
