#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>

#define MAX_FILE_SIZE 20 /* (2**64).to_s.length */
#ifndef DEBUG
    #define SEND(socket, message) send(socket, message, strlen(message), 0)
#else
    #define SEND(socket, message) fprintf(stderr, "%s", message); send(socket, message, strlen(message), 0)
#endif

int irc_user(int sockfd, char *nick, char *real_name)
{
    int err;
    char *message;

    message = calloc(11 + strlen(nick) + strlen(real_name) + 2 + 1, sizeof(char));
    sprintf(message, "USER %s x x :%s\r\n", nick, real_name);
    err = SEND(sockfd, message);
    free(message);

    return err;
}

int irc_nick(int sockfd, char *nick)
{
    int err;
    char *message;

    message = calloc(7 + strlen(nick) + 1, sizeof(char));
    sprintf(message, "NICK %s\r\n", nick);
    err = SEND(sockfd, message);
    free(message);

    return err;
}

int irc_join(int sockfd, char *channel)
{
    int err;
    char *message;

    message = calloc(8 + strlen(channel) + 1, sizeof(char));
    sprintf(message, "JOIN #%s\r\n", channel);
    err = SEND(sockfd, message);
    free(message);

    return err;
}

int irc_quit(int sockfd)
{
    return SEND(sockfd, "QUIT\r\n");
}

int irc_pong(int sockfd, char *server)
{
    int err;
    char *message;

    message = calloc(strlen("PONG ") + strlen(server) + 3, sizeof(char));
    sprintf(message, "PONG %s\r\n", server);
    err = SEND(sockfd, message);
    free(message);

    return err;
}

int irc_privmsg(int sockfd, char *remote_nick, char *content)
{
    int err;
    char *message;

    message = calloc(12 + strlen(content) + strlen(remote_nick) + 1,
            sizeof(char));
    sprintf(message, "PRIVMSG %s :%s\r\n", remote_nick, content);
    err = SEND(sockfd, message);
    free(message);

    return err;
}

int irc_notice(int sockfd, char *remote_nick, char *content)
{
    int err;
    char *message;

    message = calloc(11 + strlen(content) + strlen(remote_nick) + 1,
            sizeof(char));
    sprintf(message, "NOTICE %s :%s\r\n", remote_nick, content);
    err = SEND(sockfd, message);
    free(message);

    return err;
}

int irc_dcc_send(int sockfd, char *remote_nick, char *filename,
                 unsigned long filesize, unsigned int address, int port)
{
    int err;
    char *message, *command = "DCC SEND";

    size_t size = strlen(command) + strlen(filename) + 10 +
                  strlen("65535") + MAX_FILE_SIZE + 5 + 2;
    message = calloc(size+1, sizeof (char));
    snprintf(message, size, "%c%s %s %u %d %lu%c", '\1', command,
            filename, address, port, filesize, '\1');
    err = irc_privmsg(sockfd, remote_nick, message);

    return err;
}

int irc_dcc_accept(int sockfd, char *remote_nick, char *filename,
                 int port, unsigned long resume_offset)
{
    int err;
    char *message, *command = "DCC ACCEPT";

    size_t size = strlen(command) + strlen(filename) + 20 +
                  strlen("65535") + 5 + 2;
    message = calloc(size+1, sizeof (char));
    snprintf(message, size, "\01%s %s %d %lu\01", command,
            filename, port, resume_offset);
    err = irc_privmsg(sockfd, remote_nick, message);

    return err;
}

// vim: et ts=4 sw=4 sts=4
