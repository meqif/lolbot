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
#include <getopt.h>

#define BUFSIZE 128
#define DEFAULT_PORT "6667"

static struct option long_opts[] = {
    { "help",      no_argument,       NULL, 'h' },
    { "server",    required_argument, NULL, 's' },
    { "port",      required_argument, NULL, 'p' },
    { "path",      required_argument, NULL, 'd' },
    { NULL,        0,                 NULL,  0  }
};

static void usage() {
    fprintf(stderr, "usage: loldrop [options]\n"
                    "  -h  --help\tprint this usage and exit\n"
                    "  -s  --server\tirc server to connect\n"
                    "  -p  --port\tirc server's port (optional, defaults to 6667)\n"
                    "  -d  --path\tpath to shared directory\n");
    exit(1);
}

int main(int argc, char *argv[])
{
    int sockfd, err, ch;
    char *buf = malloc(BUFSIZE * sizeof(char));
    char *shared_path = NULL, *server = NULL, *port = DEFAULT_PORT;

    while ((ch = getopt_long(argc, argv, "hspd", long_opts, NULL)) != -1) {
        switch (ch) {
            case 's':
                server = optarg ? optarg : argv[optind];
                break;
            case 'p':
                port = optarg ? optarg : argv[optind];
                break;
            case 'd':
                shared_path = optarg ? optarg : argv[optind];
                break;
            case 'h':
            default:
                usage();
        }
    }

    argc -= optind;
    argv += optind;

    if (!shared_path || !server)
        usage();

    init_processor(shared_path);

    sockfd = create_socket(server, port);

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
