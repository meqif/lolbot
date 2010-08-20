#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <bstring.h>

#define MAX_FILE_SIZE 20 /* (2**64).to_s.length */

int irc_user(int sockfd, char *nick, char *real_name)
{
    int err;
    bstring message;

    message = bformat("USER %s x x :%s\r\n", nick, real_name);
    err = send(sockfd, bdata(message), blength(message), 0);
    bdestroy(message);

    return err;
}

int irc_nick(int sockfd, char *nick)
{
    int err;
    bstring message;

    message = bformat("NICK %s\r\n", nick);
    err = send(sockfd, bdata(message), blength(message), 0);
    bdestroy(message);

    return err;
}

int irc_join(int sockfd, char *channel)
{
    int err;
    bstring message;

    message = bformat("JOIN #%s\r\n", channel);
    err = send(sockfd, bdata(message), blength(message), 0);
    bdestroy(message);

    return err;
}

int irc_quit(int sockfd)
{
    return send(sockfd, "QUIT\r\n", 6, 0);
}

int irc_pong(int sockfd, char *server)
{
    int err;
    bstring message;

    message = bformat("PONG %s\r\n", server);
    err = send(sockfd, bdata(message), blength(message), 0);
    bdestroy(message);

    return err;
}

int irc_privmsg(int sockfd, char *remote_nick, char *content)
{
    int err;
    bstring message;

    message = bformat("PRIVMSG %s :%s\r\n", remote_nick, content);
    err = send(sockfd, bdata(message), blength(message), 0);
    bdestroy(message);

    return err;
}

int irc_notice(int sockfd, char *remote_nick, char *content)
{
    int err;
    bstring message;

    message = bformat("NOTICE %s :%s\r\n", remote_nick, content);
    err = send(sockfd, bdata(message), blength(message), 0);
    bdestroy(message);

    return err;
}

int irc_dcc_send(int sockfd, char *remote_nick, char *filename,
                 unsigned long filesize, unsigned int address, int port)
{
    int err;
    char *command = "DCC SEND";
    bstring message;

    message = bformat("\01%s %s %u %d %lu\01",
            command, filename, address, port, filesize);
    err = irc_privmsg(sockfd, remote_nick, bdata(message));
    bdestroy(message);

    return err;
}

int irc_dcc_accept(int sockfd, char *remote_nick, char *filename,
                 int port, unsigned long resume_offset)
{
    int err;
    char *command = "DCC ACCEPT";
    bstring message;

    message = bformat("\01%s %s %d %lu\01",
            command, filename, port, resume_offset);
    err = irc_privmsg(sockfd, remote_nick, bdata(message));
    bdestroy(message);

    return err;
}

// vim: et ts=4 sw=4 sts=4
