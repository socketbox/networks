#ifndef FTSERVER_H
#define FTSERVER_H

#define LSCMDNO         3 
#define GETCMDNO        5

typedef struct cmd 
{
  int cmdno;
  char filename[256];
  int dport;
} Cmd;

void clean_quit(int sfd, int e);
struct addrinfo* get_addrinfo( struct addrinfo **results );
int get_bound_socket( struct addrinfo *iface );

#endif

