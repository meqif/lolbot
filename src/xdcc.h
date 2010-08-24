/* See LICENSE file for copyright and license details. */

#ifndef XDCC_H
#define XDCC_H

struct xdcc_request {
    char *nick;
    struct file_data *file;
    unsigned long offset;
};

int ipv4_str_to_int(char *str);
int xdcc_send(struct file_data *requested_file, char *remote_nick, int sockfd);
int xdcc_list(char *remote_nick, int sockfd);

#endif /* XDCC_H */
