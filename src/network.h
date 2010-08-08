#ifndef NETWORK_H
#define NETWORK_H

void send_message(char *remote_nick, char *type, char *message, int sockfd);
int create_socket(char *hostname, char *port);
char *get_external_ip();

#endif /* NETWORK_H */
