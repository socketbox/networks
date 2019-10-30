/*
Author:       boettchc
Class:        cs372-400-f2019
Assignment:   Program 1 
Date:         October 29, 2019

Much of this code was influenced by "Beej's Guide to Network Programming." Specfically, pages 17, 
25, 28, 32, 35, 36, and 45 in the document available from https://beej.us/guide/bgnet/pdf/bgnet_usl_c_1.pdf.

Other resources paraphrased or quoted verbatim are noted in context-specific comments within the code.
*/
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <regex.h>

#ifndef DEBUG
#define DEBUG 0
#endif


//max length for user messages is 500 chars, minus handle and prompt
#define SMSG_MAX 	  488
//max length for received user messages is 500 chars
#define RMSG_MAX 	  500
//max length for user handle is 10 chars, but we will add "> \0"
#define HANDLE_LEN 	13
//tcp protocol is listed as 6 in /etc/protocols on flip
#define TCP_FLAG 		6
//pattern for parsing \quit command
#define QUIT_PATT   "^.*\\quit"


void usage()
{
	printf("chatclient [server_address] [port]\n");
	exit(0);
}


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
    close(sfd);
  //print out error
  fputs(strerror(e), stderr);
  exit(e);
}


/*
 * note: 	wasn't aware of all the heavy lifting that getaddrinfo() does when I wrote this, nor did
 *        I take into account that the user would be supplying a hostname and not an IP :|
 *
 * pre: 	n/a
 * in:		pointer to sockaddr_in and command-line args array
 * out:		n/a
 * post:  sockaddr_in ready for connecting
void build_socket_struct(struct sockaddr_in *srvr, struct in_addr *ip, char *argv[])
{
	//convert IP and check for errors before proceeding
	//if( inet_pton(AF_INET, argv[1], &(srvr->sin_addr)) )
	if( inet_pton(AF_INET, argv[1], ip) )
	{
    //put address in sockaddr_in
    srvr->sin_addr = *ip;

		//set address family to Internet
		srvr->sin_family = AF_INET;
		
		//convert port arg to int and put it in netowrk byte order
		srvr->sin_port = htons(atoi( argv[2] ));	
		
		//zero out sin_zero padding
		memset(&(srvr->sin_zero), '0', 8);
	}
}*/

/*
 * pre: 	results struct has been zeroed out
 * in:		pointer to array of addrinfo structs and command-line args array
 * out:		n/a
 * post:  results array populated by call to getaddrinfo()
 */
void build_socket_struct(struct addrinfo **results, char *argv[])
{
  if(DEBUG)
    printf("In build socket struct...");
  struct addrinfo cfg;
  memset(&cfg, 0, sizeof(struct addrinfo));
  cfg.ai_family = AF_UNSPEC;
  cfg.ai_socktype = SOCK_STREAM;
  cfg.ai_flags = AI_PASSIVE;
  cfg.ai_protocol = TCP_FLAG;

  int rv = -2;
  //pass host name and port without parsing 
  if( ( rv = getaddrinfo(argv[1], argv[2], &cfg, results) ) == -1 )
  {
    clean_quit(-1, rv); 
  }
}


/* 
 * pre:   n/a
 * in:    a pointer to a character array
 * out:   n/a
 * post:  the client's handle (with prompt) is in the provided char array
 */
void get_handle(char *handle)
{
  printf("What's your handle (maximum of 10 characters)? ");
  fgets(handle, HANDLE_LEN, stdin);
  
  //strip trailing newline: https://stackoverflow.com/a/28462221/148680 
  handle[strcspn(handle, "\n")] = 0;
  if(DEBUG)
    printf("after stcspn");
  //append "> "
  strcat(handle, "> \0");
  if(DEBUG)
    printf("User's handle: %s", handle);
}

/*
 * pre:   a handle has been selected by the user
 * in:    a message buffer and a handle
 * out:   retval, an integer indicating whether or not the user's message contained the \quit command
 *        (0 for "yes", 1 for "no")
 * post:  if user's message doesn't start with \quit, the message is sent with handle prepended
 */
int parse_msg(char *msg, char *handle)
{
  //result of regcomp
  int compres = -1;
  //function return value
  int retval = -1;

  //the struct for the compiled regex
  regex_t res;
  memset(&res, '0', sizeof(regex_t));
  size_t matchcnt = -1; 
  //regmatch_t matches[1];
  regmatch_t *matches = NULL;
  //memset(matches, '0', sizeof(regmatch_t));

  if(compres = regcomp(&res, QUIT_PATT, REG_NOSUB) == 0)
  { 
    if( regexec(&res, msg, matchcnt, matches, 0) == 0 )
      retval = 0;
    else
    {
      //create a tmp buffer
      char tmp[SMSG_MAX];
      memset(tmp, '\0', SMSG_MAX);
      strcat(tmp, handle);
      strcat(tmp, msg);
      memcpy(msg, tmp, sizeof(char)*SMSG_MAX);
      retval = 1;
    }
  }
  else
  {
    //create buffer for error msg
    int buffsz = 256;
    char errbuff[buffsz];
    regerror(compres, &res, errbuff, buffsz);
    fprintf(stderr, "Failed to compile regex: %s\n", errbuff);
    exit(compres);
  }
  return retval;
}


//TODO connection refused needs to error out and exit
int main(int argc, char *argv[])
{
	if(argc != 3 )
	{
		usage();
	}

	//user's handle	
	char handle[HANDLE_LEN];
  memset(handle, '\0', HANDLE_LEN);
  get_handle(handle);

	/*create sockaddr_in struct and populate it
	struct sockaddr_in sconn;
  struct in_addr ip4;	
  memset(&ip4, 0, sizeof(struct in_addr));	
  memset(&sconn, 0, sizeof(struct sockaddr_in));	
  */
  struct addrinfo *results, *h;
  build_socket_struct(&results, argv);

	//create the socket file descriptor; 
	int sckt = socket(PF_INET, SOCK_STREAM, 6);

  struct addrinfo host;
  memset(&host, 0, sizeof(struct addrinfo));
 
  /* There's got to be a better way to do this... */
  for(h = results; h != NULL; h = h->ai_next)
  {
	  if( connect(sckt, h->ai_addr, h->ai_addrlen) == -1)
    {
      close(sckt);
      fprintf(stderr, "%s", strerror(errno));	
      continue;
    }
    if(DEBUG)
      fprintf(stderr, "Connecting to %i", *(h->ai_canonname)); 
    break;
  }

  //add one to maximum for null byte
  char send_msg_buff[SMSG_MAX+1];
  char recv_msg_buff[RMSG_MAX];
  do
  {
    //clear the message buffers
    memset(recv_msg_buff, '\0', RMSG_MAX);
    memset(send_msg_buff, '\0', SMSG_MAX);
    //print handle as prompt 
    printf("%s", handle);
	  fgets(send_msg_buff, SMSG_MAX, stdin);
    
    if(parse_msg(send_msg_buff, handle))
    {
      if(DEBUG)
        printf("strlen of send buff: %lu", strlen(send_msg_buff));
      send(sckt, send_msg_buff, strlen(send_msg_buff), 0); 
    }
    else
      clean_quit(sckt, 1);
    
    recv(sckt, recv_msg_buff, RMSG_MAX, 0);
    fprintf(stdout, "%s\n", recv_msg_buff);
  }
  while(sckt);
  
  return 0;
}

