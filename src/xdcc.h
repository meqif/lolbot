/* See LICENSE file for copyright and license details. */

#ifndef XDCC_H
#define XDCC_H

int xdcc_send(struct file_data requested_file, char *remote_nick, int sockfd);
int xdcc_list(char *remote_nick, int sockfd);

#endif /* XDCC_H */
