/*
Author:       boettchc
Class:        cs372-400-f2019
Assignment:   Program 1 
Date:         October 29, 2019

Much of this code was influenced by "Beej's Guide to Network Programming." Specfically, pages 17, 
25, 28, 32-36, and 45 in the document available from https://beej.us/guide/bgnet/pdf/bgnet_usl_c_1.pdf.

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

#ifndef DEBUG1
#define DEBUG1 0
#endif

#ifndef DEBUG2
#define DEBUG2 0
#endif


//max length for user messages is 500 chars, plus handle
#define SMSG_MAX 	  513
//max length for received user messages is 500 chars
#define RMSG_MAX 	  513
//max length for user handle is 10 chars, but we will add "> \0"
#define HANDLE_LEN 	13
//pattern for parsing \quit command
#define QUIT_PATT   "^.*\\\\quit"


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
  {
    if(DEBUG2)
      fprintf(stderr, "Prior to calling shutdown and close.\n");
    if( shutdown(sfd, SHUT_RDWR) < 0)
    {
      if(DEBUG2)
      {
        fprintf(stderr, "After calling shutdown.\n");
        fprintf(stderr, "Shutdown failed: %s", strerror(errno)); 
      }
    }
    if(DEBUG2)
        fprintf(stderr, "Prior to calling close.\n");
    if( close(sfd) < 0)
    {
      if(DEBUG2)
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


/*
 * pre: 	results struct has been zeroed out
 * in:		pointer to array of addrinfo structs and command-line args array
 * out:		n/a
 * post:  results array populated by call to getaddrinfo()
 */
void build_socket_struct(struct addrinfo **results, char *argv[])
{
  if(DEBUG1)
  {
    fprintf(stderr, "In build socket struct...\n");
  }
  struct addrinfo cfg;
  memset(&cfg, 0, sizeof(struct addrinfo));
  cfg.ai_family = AF_INET;
  cfg.ai_socktype = SOCK_STREAM;
  cfg.ai_flags = AI_PASSIVE;
  cfg.ai_protocol = IPPROTO_TCP;

  int rv = -2;
  //pass host name and port without parsing 
  if( ( rv = getaddrinfo(argv[1], argv[2], &cfg, results) ) != 0 )
  {
    if(DEBUG1)
    {
      fprintf(stderr, "getaddrinfo %s %s failed:", argv[1], argv[2]);
    }
    clean_quit(-1, rv); 
  }
}


/* 
 * pre:   char array argument has been filled with null bytes
 * in:    a pointer to a character array
 * out:   n/a
 * post:  the client's handle (with prompt) is in the provided char array
 */
void get_handle(char *handle)
{
  if(DEBUG1)
    fprintf(stderr, "In get_handle...\n");
  printf("What's your handle (maximum of 10 characters)? ");
  //writes 10 chars to handle and then puts a null byte at the end 
  fgets(handle, HANDLE_LEN-2, stdin);
  
  if(DEBUG1)
    fprintf(stderr, "After fgets\n");
  
  //strip trailing newline: https://stackoverflow.com/a/28462221/148680 
  handle[strcspn(handle, "\n")] = 0;
  if(DEBUG1)
    fprintf(stderr, "after stcspn\n");
  //append "> "
  strcat(handle, "> \0");
  if(DEBUG1)
  {    
    fprintf(stderr, "User's handle: %s\n", handle);
    fflush(stderr);
  }
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
 
  //tmp array for strcat operations
  char tmp[SMSG_MAX];

  if( (compres = regcomp(&res, QUIT_PATT, REG_NOSUB)) == 0)
  { 
    if( regexec(&res, msg, -1, NULL, 0) == 0 )
      retval = 0;
    else
    {
      //create a tmp buffer
      memset(tmp, '\0', sizeof(char)*SMSG_MAX);
      strcat(tmp, handle);
      strcat(tmp, msg);
      memcpy(msg, tmp, sizeof(char)*SMSG_MAX);
      retval = 1;
    }
    regfree(&res);
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


int main(int argc, char *argv[])
{
  //check that the user has supplied the correct number of arguments	
  if(argc != 3 )
	{
		usage();
	}

	//user's handle	
	char handle[HANDLE_LEN];
  memset(handle, '\0', HANDLE_LEN);
  get_handle(handle);

  //config socket
  struct addrinfo results, h;
  struct addrinfo *res = &results;
  struct addrinfo *hptr = &h;
  memset(&results, 0, sizeof(struct addrinfo));
  memset(&h, 0, sizeof(struct addrinfo));
  build_socket_struct(&res, argv);

	//create the socket file descriptor; 
	int sckt = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

  struct addrinfo host;
  memset(&host, 0, sizeof(struct addrinfo));

  if(DEBUG1)
    fprintf(stderr, "Prior to looping through results..\n");

  int success = 0;
  /* There's got to be a better way to do this... */
  for(hptr = res; hptr != NULL; hptr = hptr->ai_next)
  {
	  if( connect(sckt, hptr->ai_addr, hptr->ai_addrlen) == -1)
    {
      if(DEBUG1)
        fprintf(stderr, "After connect() failed.\n");
      close(sckt);
      fprintf(stderr, "%s\n", strerror(errno));	
      continue;
    }
    else
      success = 1;
    break;
  }
  if(success == 0)
  {
    fprintf(stderr, "Failed to connect. Exiting: %s.\n", strerror(errno));
    exit(errno); 
  }
  freeaddrinfo(res);

  //clear the message buffers
  char send_msg_buff[SMSG_MAX] = {'\0'};
  char recv_msg_buff[RMSG_MAX] = {'\0'};
  do
  {
    //print handle as prompt 
    printf("%s", handle);
    fflush(stdout);
	  fgets(send_msg_buff, SMSG_MAX, stdin);
    
    if(parse_msg(send_msg_buff, handle))
    {
      if(DEBUG1)
        printf("strlen of send buff: %lu", strlen(send_msg_buff));
      send(sckt, send_msg_buff, strlen(send_msg_buff), 0); 
    }
    else
    {
      if (DEBUG2)
        fprintf(stderr, "Calling clean_quit.");
      clean_quit(sckt, 0);
    }
    
    recv(sckt, recv_msg_buff, RMSG_MAX, 0);
    fprintf(stdout, "%s\n", recv_msg_buff);
  }
  while(sckt);
    
  return 0;
}

