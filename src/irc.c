#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>

#define SEND(socket, message) send(socket, message, strlen(message), 0)

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

// vim: et ts=4 sw=4 sts=4
