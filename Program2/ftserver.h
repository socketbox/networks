#ifndef FTSERVER_H
#define FTSERVER_H
#include <netdb.h>

void clean_quit(int sfd, int e);
struct addrinfo* get_addrinfo( struct addrinfo **results );
int get_bound_socket( struct addrinfo *iface );

#endif

