#include "xdcc.h"
#include "network.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <getopt.h>

#define BUFSIZE 128
#define DEFAULT_PORT "6667"

#define SEND(socket, message) send(socket, message, strlen(message), 0)

static struct option long_opts[] = {
    { "help",      no_argument,       NULL, 'h' },
    { "server",    required_argument, NULL, 's' },
    { "port",      required_argument, NULL, 'p' },
    { "path",      required_argument, NULL, 'd' },
    { "channel",   required_argument, NULL, 'c' },
    { "nick",      required_argument, NULL, 'n' },
    { NULL,        0,                 NULL,  0  }
};

static void usage() {
    fprintf(stderr, "usage: loldrop [options]\n"
                    "  -h  --help\tprint this usage and exit\n"
                    "  -s  --server\tirc server to connect\n"
                    "  -p  --port\tirc server's port (optional, defaults to 6667)\n"
                    "  -d  --path\tpath to shared directory\n"
                    "  -c  --channel\tchannel to join\n"
                    "  -n  --nick\tnickname to be used\n"
            );
    exit(1);
}

int main(int argc, char *argv[])
{
    int sockfd, err, ch;
    char *buf = malloc(BUFSIZE * sizeof(char));
    char *shared_path = NULL, *server = NULL, *port = DEFAULT_PORT,
         *channel = NULL, *nick = NULL, *message, *ptr;

    while ((ch = getopt_long(argc, argv, "hspdcn", long_opts, NULL)) != -1) {
        ptr = optarg ? optarg : argv[optind];

        if (!ptr) {
            fprintf(stderr, "Messed up argument, maybe your shell is responsible?\n");
            return 1;
        }

        switch (ch) {
            case 's':
                server = ptr;
                break;
            case 'p':
                port = ptr;
                break;
            case 'd':
                shared_path = ptr;
                break;
            case 'n':
                nick = ptr;
                break;
            case 'c':
                channel = ptr;
                if (*channel == '#')
                    channel++;
                break;
            case 'h':
            default:
                usage();
        }
    }

    argc -= optind;
    argv += optind;

    if (!shared_path || !server || !channel || !nick)
        usage();

    init_processor(shared_path);

    sockfd = create_socket(server, port);

    FILE *interwebs = fdopen(sockfd, "r+");

    message = calloc(11 + 2 * strlen(nick) + 2 + 1, sizeof(char));
    sprintf(message, "USER %s x x :%s\r\n", nick, nick);
    err = SEND(sockfd, message);
    printf("%d\n", err);
    free(message);

    message = calloc(7 + strlen(nick) + 1, sizeof(char));
    sprintf(message, "NICK %s\r\n", nick);
    err = SEND(sockfd, message);
    printf("%d\n", err);
    free(message);

    sleep(1);

    message = calloc(8 + strlen(channel) + 1, sizeof(char));
    sprintf(message, "JOIN #%s\r\n", channel);
    err = SEND(sockfd, message);
    printf("%d\n", err);
    free(message);

    while (1) {
        memset(buf, 0, BUFSIZE);
        fgets(buf, BUFSIZE, interwebs);
        printf("%s", buf);
        xdcc_process(buf, sockfd);
    }

    err = SEND(sockfd, "QUIT\r\n");
    printf("%d\n", err);

    fclose(interwebs);
    close(sockfd);
    free(buf);

    return 0;
}

// vim: et ts=4 sw=4 sts=4
