/********************************************************************************** 
 * 
 * Note to grader: almost all of the code in this file was taken from a 
 * project that was recently completed for CS344
 * 
**********************************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <limits.h>
#include <errno.h>
#include <wait.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/ip.h>
#include "cmdfork.h"
#include "ftserver.h"


/*
 * duplicate stdout to the socket
 */
void redir_stderr()
{
  int devnull = open("/dev/null", O_WRONLY);
  int dupres = dup2(devnull, STDERR_FILENO);
  if(dupres == -1)
  { 
    perror("Failed to duplicate stdserr to /dev/null"); 
    exit(1); 
  }
}


/*
 * duplicate stdout to the socket
 */
void redir_stdout(int sfd)
{
  int dupres = dup2(sfd, STDOUT_FILENO);
  if(dupres == -1)
  { 
    perror("Failed to duplicate stdout to socket fd"); 
    exit(1); 
  }
  //after dup'ing, we no longer need this
  close(sfd);
}


int send_file(Cmd cs, struct sockaddr_in *client)
{
  int em, sigstatus, datafd, send_file_res;
  datafd = sigstatus = em = send_file_res = INT_MIN;
  pid_t spawnpid = INT_MIN;

  if((datafd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
  { 
    perror("Failed to create socket for client data connection."); 
    exit(EXIT_FAILURE);
  }

  fprintf(stdout, "Sending %s to %s:%i\n", cs.filename, cs.client_hostname, cs.dport);
  fflush(stdout);
  spawnpid = fork();
  switch(spawnpid)
  {
    case -1:
      perror("Fork in run_fg_child returned error");
      exit(1);
      break;

      //in child 
    case 0:
      {
        int cxres = INT_MAX; 
        socklen_t scklen = sizeof( *client );
        if((cxres = connect(datafd, (struct sockaddr *)client, scklen) < 0))
        {
          perror("Failed to connet prior to exec.");
          exit(1);
        }
        redir_stdout(datafd);
        redir_stderr();
        execlp("cat", "cat", cs.filename, NULL);
        perror("Execution of cat failed. Exiting.");
        send_file_res = errno; 
        break;
      } 
      //in parent
    default:
      //don't need the fd after forking 
      close(datafd);
      spawnpid = waitpid(spawnpid, &em, 0); 
      if (WIFEXITED(em))
      {
        sigstatus = WEXITSTATUS(em);
        send_file_res = sigstatus; 
        if(DEBUG){fprintf(stderr, "Exiting child with status %i\n", sigstatus);}
      }
      else if(WIFSIGNALED(em))
      {
        sigstatus = WTERMSIG(em);
        if(DEBUG){fprintf(stderr, "Child terminated by signal %i\n", sigstatus);}
      }
      else
      {
        fprintf(stderr, "Why are we here? Status/Signal: %i\n", sigstatus);
      }
      break;
  }
  return send_file_res;  
}


int send_ls(Cmd cs, struct sockaddr_in *client)
{
  int em, sigstatus, datafd;
  datafd = sigstatus = em = INT_MIN;
  pid_t spawnpid = INT_MIN;
  if((datafd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
  { 
    perror("Failed to create socket for client data connection."); 
    exit(EXIT_FAILURE);
  }
  fprintf(stdout, "Sending directory contents to %s:%i\n", cs.client_hostname, cs.dport);
  fflush(stdout);
  spawnpid = fork();
  switch(spawnpid)
  {
    case -1:
      perror("Fork in run_fg_child returned error");
      exit(1);
      break;
   
    //in child 
    case 0:
      {
        //redir_stdout(datafd);
        int cxres = INT_MAX; 
        socklen_t scklen = sizeof( *client );
        if((cxres = connect(datafd, (struct sockaddr *)client, scklen) < 0))
        {
          perror("Failed to connet prior to exec.");
          exit(1);
        }
        redir_stdout(datafd);
        execlp("ls", "ls", NULL);
        perror("Execution of ls failed. Exiting.");
        break;
      } 
    //in parent
    default:
      //don't need this after redirect 
      close(datafd);
      spawnpid = waitpid(spawnpid, &em, 0); 
      if (WIFEXITED(em))
      {
        sigstatus = WEXITSTATUS(em);
        if(DEBUG){fprintf(stderr, "Exiting child with status %i\n", sigstatus);}
      }
      else if(WIFSIGNALED(em))
      {
        sigstatus = WTERMSIG(em);
        if(DEBUG){fprintf(stderr, "Child terminated by signal %i\n", sigstatus);}
      }
      else
      {
        if(DEBUG){fprintf(stderr, "Why are we here? Status/Signal: %i\n", sigstatus);}
      }
      //close data connection
      close(datafd);
      break;
  }
  return spawnpid;  
}


int execute_cmd(const Cmd cs, struct sockaddr *client)
{
  int retval = 1;
  //modify the port field in the local copy of the client struct 
  struct sockaddr_in *claddrin = (struct sockaddr_in *)client;
  //grrrr...overlooking this had me flumoxed for a fair bit of time
  claddrin->sin_port = htons(cs.dport);

  //if client issued "-l"
  if(cs.cmdno == LSCMDNO)
  {
    //exec ls and send results
    fprintf(stdout, "List directory requested on port %i\n", cs.dport);
    retval = send_ls(cs, claddrin);
  }
  //if client issued "-g"
  if(cs.cmdno == GETCMDNO)
  {
    fprintf(stdout, "File \"%s\" requested on port %i.\n", cs.filename, cs.dport);
    //exec ls and send results
    retval = send_file(cs, claddrin);
  }
  return retval;
}

