/* See LICENSE file for copyright and license details. */

#ifndef IRC_H
#define IRC_H

int irc_user(int sockfd, char *nick, char *real_name);
int irc_nick(int sockfd, char *nick);
int irc_join(int sockfd, char *channel);
int irc_quit(int sockfd);

#endif /* IRC_H */
