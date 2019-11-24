/*
 * Note to grader: much of this is influenced by Kerrisk's "The Linux Programming Interface", Chapter 59,
 * as well as on recent projects completed for cs344 and the chat server project completed for this class,
 * CS372
 */
#include <sys/socket.h>
#include <inttypes.h>
#include <netdb.h>
#include <netinet/ip.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include "cmdfork.h"

#define LSCMD        "-l" 
#define LSCMDNO         3 
#define GETCMD       "-g" 
#define GETCMDNO        5
#define BACKLOG         4 
#define BAD_ARGS      -10
#define BAD_PORT      -11
#define CMD_LEN       280

#ifndef DEBUG
#define DEBUG           0
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
  if(DEBUG){fprintf(stderr, "In clean_quit...");}
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


/*
 * pre: 	results struct has been zeroed out
 * in:		pointer to array of addrinfo structs and command-line args array
 * out:		n/a
 * post:  results array populated by call to getaddrinfo()
 */
void get_addrinfo_arr(struct addrinfo *results[], char *port)
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
  //for the command cx, this would be better as a datagram socket
  cfg.ai_socktype = SOCK_STREAM;
  //we want to listen; we'll designate the port by number, not service name
  cfg.ai_flags = AI_PASSIVE | AI_NUMERICSERV;
  //don't understand the point in leaving this 0, when we know we want TCP
  cfg.ai_protocol = IPPROTO_TCP;

  int rv = -2;
  //pass host name and port without parsing 
  if( ( rv = getaddrinfo(NULL, port, &cfg, results) ) != 0 )
  {
    perror("Fatal. Did not obtain socket");
    clean_quit(-1, rv); 
  }
}


struct addrinfo* get_addrinfo( struct addrinfo **results )
{
  //temp. file descriptor
  int fd = INT_MIN;
  struct addrinfo *the_one, *dc;
  the_one = dc = NULL;
  for(the_one = *results; the_one != NULL; the_one = the_one->ai_next)
  {
    fd = socket(the_one->ai_family, the_one->ai_socktype, the_one->ai_protocol);
    if(fd == -1)
      continue;
    else
    {
      //create a deep copy and send the copy back
      size_t addrinfosz = sizeof( struct addrinfo );
      dc = malloc(addrinfosz);
      memcpy(dc, the_one, addrinfosz); 
      //TODO  
      //close(fd);
    }
  } 
  return dc;
}

int get_bound_socket( struct addrinfo *iface )
{
  int fd, opts, ssores, bindres;
  bindres = fd = opts = ssores = INT_MIN;

  if((fd = socket(iface->ai_family, iface->ai_socktype, iface->ai_protocol)) > -1)
  {
    //Kerrisk advises that the server should reuse the port to which it's being bound
    if((ssores = setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opts, sizeof(opts))) == -1)
      clean_quit(fd, errno);

    //success should set bindres to 0 and exit the loop
    if((bindres = bind(fd, iface->ai_addr, iface->ai_addrlen)) != 0)
      ;
      //TODO close(fd);
  }
  else
  {
    perror("Failed to get socket");
  }
  return fd;
}

/*
 * returns 3 for "-l"; 5 for "-g"; 0 for bad length; -1 for bad string
 */
Cmd confirm_cmd(int bytes, char *buff, int cxfd)
{
  Cmd cs = {0};

  char *tkstart = NULL;
  char *tkarg = NULL;
  char *tkstate = NULL;
  char **tks = &tkstate;
  char *delims = " \n";
  char *strtoi = NULL; 
  int len = -1;
  int port = INT_MIN;
  
  //get the command
  tkstart = strtok_r(buff, delims, tks);
  if(tkstart)
  {
    len = strlen(tkstart);
    if(len != 2) 
    {
      send(cxfd, "Bad Command Length: commands shoule be no more than two characters.\n", 69, 0);
      clean_quit(cxfd, errno);
    }
    else 
    {
      //check for list command; valid cmd is of length 2
      if(strncmp(LSCMD, tkstart, 2)==0)
      {
        cs.cmdno = LSCMDNO;
        if((tkarg = strtok_r(NULL, delims, tks)) != NULL)
        {
          //next argument after '-l' should be a port number
          port = strtoimax(tkarg, &strtoi, 10);
          if(!(port > 0 && port < 65536))
          {
            send(cxfd, "Bad Data Port Number.\n", 22, 0);
            clean_quit(cxfd, errno);
          }
          cs.dport = port;
        }
      }
      //check for get command; valid cmd is of length 2
      else if( strncmp(GETCMD, tkarg, 2) == 0)
      {
        cs.cmdno = GETCMDNO;
        if((tkarg = strtok_r(NULL, delims, tks)) != NULL)
        {
          //we will let a bad filename fail later
          int fnlen = strlen(tkarg);
          strncpy(cs.filename, tkarg, fnlen); 
       
          //this should get us the port
          if((tkarg = strtok_r(NULL, delims, tks)) != NULL)
          {
            //next argument after '-l' should be a port number
            port = strtoimax(tkarg, &strtoi, 10);
            if(!(port > 0 && port < 65536))
            {
              send(cxfd, "Bad Data Port Number.\n", 22, 0);
              clean_quit(cxfd, errno);
            }
            cs.dport = port;
          }
        } 
      }  
      else
      {
        send(cxfd, "Bad Command: valid commands are -l and -g\n", 43, 0);
        clean_quit(cxfd, errno);
      }
    }
  }
  return cs;
}


void enter_cmd_loop(int cmdfd, char *port)
{
  int cxfd, gnires, cmdbytes;
  cxfd = cmdbytes = gnires = INT_MIN;
  struct sockaddr claddr = {0};
  unsigned int claddrlen = sizeof(struct sockaddr);
  char cxinghost[NI_MAXHOST] = {'\0'};
  char cxingport[NI_MAXSERV] = {'\0'};
  const char *errmsg = NULL;

  while(1)
  {
    //use sockaddr_in because we want to reuse it to reconnect to the client on the data port
    if((cxfd = accept(cmdfd, (struct sockaddr *)&claddr, &claddrlen)) == -1)
      close(cxfd);
    else
    {
      //struct sockaddr_in recipaddr = (sockaddr_in)claddr;
      printf("DEBUG"); 
      /* get the hostname the client is connecting from; NI_* constants defined in netdb.h; options
        NI_NOFQDN because most likely a local subnet connection and NI_NUMERICSERV to avoid service
        port lookup, since we'll be using a non-standard port */
      if( (gnires = getnameinfo( (struct sockaddr *)&claddr, claddrlen, cxinghost, NI_MAXHOST, cxingport, NI_MAXSERV, NI_NOFQDN | NI_NUMERICSERV)) == 0)
        fprintf(stdout, "Connection from %s\n", cxinghost);
      else
      {
        errmsg = gai_strerror(gnires);
        fprintf(stderr, "getnameinfo() error: %s\n", errmsg);
        exit(EXIT_FAILURE);
      }
      
      char cmdbuff[CMD_LEN];
      //cmd loop
      while(1)
      {
        //zero out the buffer
        memset(cmdbuff, '\0', CMD_LEN);
        cmdbytes = recv(cxfd, cmdbuff, CMD_LEN+1, 0);
        if( cmdbytes > 0)
        {
          Cmd cs = confirm_cmd(cmdbytes, cmdbuff, cxfd);
          if(cs.cmdno > 0)
          {
            execute_cmd(cs, &claddr);
            //call to setup data stream in fork'd child
            //you'll need to modify args to accept loop such that you can establish a new cx in
            //a different function
          }
        }
      }
    }
    close(cxfd); 
  }
}


int main(int argc,char *argv[])
{
  int ckargres, sckt;
  sckt = ckargres = INT_MIN;
  
  char *port = NULL;

  if(!(ckargres = check_args(argc, argv)))
  {
    print_usage();
    clean_quit(-1, ckargres);
  }
  else
    port = argv[1];

  //get a socket address to create a socket from
  struct addrinfo *aifos[] = {0};
  get_addrinfo_arr(aifos, port);

  //we want a deep copy of this socket address to reuse it EDIT: no, no, no...
  struct addrinfo *iface = NULL;
  iface = get_addrinfo(aifos);
  //we've got a reusable socket address; burn the tmp array EDIT: no, no, no...
  freeaddrinfo(*aifos); 

  //bind to the socket
  sckt = get_bound_socket(iface);
  free(iface);
  
  //start listening
  if( listen(sckt, BACKLOG) == 0)
    fprintf(stdout, "Server open on %s\n", port);
  
  //TODO implement SIGCHLD handler?

  //accept() and block until client connects
  enter_cmd_loop(sckt, port);
}


