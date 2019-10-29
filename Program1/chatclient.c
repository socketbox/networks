/*
Author:       boettchc
Class:        cs372-400-f2019
Assignment:   Program 1 
Date:         October 29, 2019

Much of this code was influenced by "Beej's Guide to Network Programming." Specfically, pages 17, 
25, 28, 32, 35, and 36 in the document available from https://beej.us/guide/bgnet/pdf/bgnet_usl_c_1.pdf.

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

//max length for user messages is 500 chars, plus null byte
#define MSG_MAX 	  500
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
 * pre: 	n/a
 * in:		pointer to sockaddr_in and command-line args array
 * out:		n/a
 * post:  sockaddr_in ready for connecting
 * note: 	wasn't aware of all the heavy lifting that getaddrinfo() does when I wrote this, nor did
 *        I take into account that the user would be supplying a hostname and not an IP :|
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
}
*/
/*
 * pre: 	results struct has been zeroed out
 * in:		pointer to array of addrinfo structs and command-line args array
 * out:		n/a
 * post:  results array populated by call to getaddrinfo()
 */
void build_socket_struct(struct addrinfo **results, char *argv[])
{
  struct addrinfo cfg;
  memset(&cfg, 0, sizeof(struct addrinfo);
  cfg.ai_family = AF_UNSPEC;
  cfg.ai_socktype = SOCK_STREAM;
  cfg.ai_flags = AI_PASSIVE;
  cfg.ai_protocol = TCP_FLAG;

  //convert port to network byte order
  uint16_t port = htons(atoi( argv[2] ));
  if(rv = getaddrinfo(argv[1], port, &cfg, results != 0)
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
  //append "> "
  strcat(handle, "> \0");
  if(DEBUG)
    printf("User's handle: %s", handle);
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


int parse_msg(char *msg)
{
  regex_t pattern;
  memset(&pattern, '0', sizeof(regex_t));
  
  if(regcomp(&pattern, QUIT_PATT))
    printf("foo");
  else
    printf("bar");
}

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
  build_socket_struct(&sconn, &ip4, argv);

	//create the socket file descriptor; 
	int sckt = socket(PF_INET, SOCK_STREAM, 6);
	
	//connect!
	if( connect(sckt, (struct sockaddr *)&sconn, sizeof sconn) < 0 )
  {
    clean_quit(sckt, errno);	
  }

  //add one to maximum for null byte
  char msg_buff[MSG_MAX+1];
  do
  {
    memset(msg_buff, '\0', MSG_MAX);
    printf("%s", handle);
	  fgets(msg_buff, MSG_MAX, stdin);
    if(parse_msg(msg_buff))
      send(sckt, msg_buff, strlen(msg_buff), 0); 
    else
      clean_quit(sckt, 0);
  }
  while(sckt);
}









