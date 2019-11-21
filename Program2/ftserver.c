#include <sys/socket.h>
#include <netdb.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

#define BAD_ARGS      -10
#define BAD_PORT      -11

#ifndef DEBUG
#define DEBUG         0
#endif


/*
 * pre:   n/a
 * in:    integers for a socket file descriptor and errno; a negative value for sfd avoids
 *        attempting to close the socket
 * out:   n/a
 * post:  socket is closed, user is shown any relevant error messages, and application exits
 */
void clean_quit(int sfd, int e)
{
  //close the socket
  if(sfd > -1)
  {
    if(DEBUG)
      fprintf(stderr, "Prior to calling shutdown and close.\n");
    if( shutdown(sfd, SHUT_RDWR) < 0)
    {
      if(DEBUG)
      {
        fprintf(stderr, "After calling shutdown.\n");
        fprintf(stderr, "Shutdown failed: %s", strerror(errno)); 
      }
    }
    if(DEBUG)
        fprintf(stderr, "Prior to calling close.\n");
    if( close(sfd) < 0)
    {
      if(DEBUG)
      {
        fprintf(stderr, "After calling close.\n");
        fprintf(stderr, "Close failed: %s", strerror(errno)); 
      }
    }
  }
  //print out error
  if(e != 0)
    fputs(strerror(e), stderr);
  exit(e);
}


int check_args(int argc, char *argv[])
{
  int retval = 1;
  char *p = NULL;

  if(argc != 2)
    retval = BAD_ARGS;
  else
  {
    p = argv[1];
    int port = atoi(p);
    if(port < 1024 || port > 65535)
      retval = BAD_PORT;
  }
  return retval;
}


void print_usage()
{
  fprintf(stderr, "%s\n", "ftserver server_port");
}


int setup_cmd_cx(int port)
{
  int sckt = INT_MIN;
  int bndres = INT_MIN;

  if(sckt = socket(AF_INET, SOCK_STREAM, 0) == -1 )
  {
    perror("Fatal. Error creating socket");
    exit(errno);
  }
  //TODO bndres = bind(sckt,  
}


/*
 * pre: 	results struct has been zeroed out
 * in:		pointer to array of addrinfo structs and command-line args array
 * out:		n/a
 * post:  results array populated by call to getaddrinfo()
 */
void get_sockets(struct addrinfo *results[], char *port)
{
  if(DEBUG)
  {
    fprintf(stderr, "In build socket struct...\n");
  }
  struct addrinfo cfg;
  memset(&cfg, 0, sizeof(struct addrinfo));
  cfg.ai_canonname = NULL;
  cfg.ai_addr = NULL;
  cfg.ai_next = NULL;
  cfg.ai_family = AF_INET;
  cfg.ai_socktype = SOCK_STREAM;
  cfg.ai_flags = AI_PASSIVE;
  cfg.ai_protocol = IPPROTO_TCP;

  int rv = -2;
  //pass host name and port without parsing 
  if( ( rv = getaddrinfo(NULL, port, &cfg, results) ) != 0 )
  {
    perror("Fatal. Did not obtain socket");
    clean_quit(-1, rv); 
  }
}




int main(int argc,char *argv[])
{
  int ckargres = INT_MIN;
  int sckt = INT_MIN;
  char *port = NULL;

  if(!(ckargres = check_args(argc, argv)))
  {
    print_usage();
    clean_quit(-1, ckargres);
  }
  else
    port = argv[1];

  struct addrinfo *aifos[] = {0};
  get_sockets(aifos, port);

  //sckt = setup_cmd_cx(port);
}



