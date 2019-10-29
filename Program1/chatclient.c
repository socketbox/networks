


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


#define HANDLE_LEN 	11
//tcp protocol is listed as 6 in /etc/protocols on flip
#define TCP_FLAG 		6


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
 * note: 	wasn't aware of all the heavy lifting that getaddrinfo() does when I wrote this :|
 */
void build_socket_struct(struct sockaddr_in *srvr, char *argv[])
{
	//convert IP and check for errors before proceeding
	if( inet_pton(AF_INET, argv[1], &(srvr->sin_addr)) )
	{
		//set address family to Internet
		srvr->sin_family = AF_INET;
		
		//convert port arg to int and put it in netowrk byte order
		srvr->sin_port = htons(atoi( argv[2] ));	
		
		//zero out sin_zero padding
		memset(&(srvr->sin_zero), '0', 8);
	}
}


void get_handle(char *handle)
{
	fgets(handle, HANDLE_LEN, stdin);
}


int main(int argc, char *argv[])
{
	if(argc != 3 )
	{
		usage();
	}

	//user's handle	
	char handle[11];
	get_handle(handle);

	//create sockaddr_in struct and populate it
	struct sockaddr_in *sconn;
	memset(&sconn, 0, sizeof sconn);	
	build_socket_struct(sconn, argv);

	//create the socket file descriptor; 
	int sckt = socket(PF_INET, SOCK_STREAM, 6);
	
	//connect!
	int conn = connect(sckt, (struct sockaddr *)sconn, sizeof sconn);	

	char *msg = "This is my message to you.";
	send(sckt, msg, strlen(msg), 0); 
	
}









/*(from socket import *

HANDLE="HocusPocus> "

while True:
    #serverName = '128.193.54.182'
    serverName = '127.0.0.1'
    #serverPort = 12000
    serverPort = 2503
    clientSocket = socket(AF_INET, SOCK_STREAM)
    clientSocket.connect((serverName,serverPort))

    sentence = input(HANDLE)
    print("You provided: " + sentence)
    enc_sent = str.encode(HANDLE+sentence);
    clientSocket.send(enc_sent)
    modifiedSentence = clientSocket.recv(500)
    print(modifiedSentence)
    clientSocket.close()

*/
