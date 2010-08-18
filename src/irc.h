/* See LICENSE file for copyright and license details. */

#ifndef IRC_H
#define IRC_H

int irc_user(int sockfd, char *nick, char *real_name);
int irc_nick(int sockfd, char *nick);
int irc_join(int sockfd, char *channel);
int irc_quit(int sockfd);
int irc_pong(int sockfd, char *server);
int irc_privmsg(int sockfd, char *remote_nick, char *content);
int irc_notice(int sockfd, char *remote_nick, char *content);
int irc_dcc_send(int sockfd, char *remote_nick, char *filename,
                 unsigned long filesize, unsigned int address, int port);

#endif /* IRC_H */
