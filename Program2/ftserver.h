#ifndef FTSERVER_H
#define FTSERVER_H

#include <netdb.h>

#define LSCMDNO         3 
#define GETCMDNO        5

#ifndef DEBUG
#define DEBUG           0
#endif

typedef struct cmd 
{
  int cmdno;
  char filename[256];
  char client_hostname[NI_MAXHOST];
  int cmdport;
  int dport;
} Cmd;

void clean_quit(int sfd, int e);
//struct addrinfo* get_addrinfo( struct addrinfo **results );
//int get_bound_socket( struct addrinfo *iface );

#endif

