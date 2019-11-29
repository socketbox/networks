/********************************************************************************** 
 * 
 * Note to grader: almost all of the code in this file was taken from a 
 * project that was recently completed for CS344
 * 
**********************************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <wait.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/ip.h>
#include "ftserver.h"


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
        perror("Execution of foreground command failed. Exiting.");
        break;
      } 
    //in parent
    default:
      close(datafd);
      //if(DEBUG){fprintf(stderr, "In parent's case; spawnpid: %i\n", spawnpid);}
      spawnpid = waitpid(spawnpid, &em, 0); 
      if (WIFEXITED(em))
      {
        //if(SIGDEBUG){fprintf(stderr, "The process (%i) exited normally\n", spawnpid);}
        sigstatus = WEXITSTATUS(em);
      }
      else if(WIFSIGNALED(em))
      {
        //if(DEBUG){fprintf(stderr, "The process was signalled.\n");}
        sigstatus = WTERMSIG(em);
        fprintf(stderr, "terminated by signal %i\n", sigstatus);
      }
      else
      {
        fprintf(stderr, "Why are we here? Status/Signal: %i\n", sigstatus);
      }
      break;
  }
  return spawnpid;  
}


int execute_cmd(const Cmd cs, struct sockaddr *client)
{
  //modify the port field in the local copy of the client struct 
  struct sockaddr_in *claddrin = (struct sockaddr_in *)client;
  //grrrr
  claddrin->sin_port = htons(cs.dport);

  //if client issued "-l"
  if(cs.cmdno == LSCMDNO)
  {
    //exec ls and send results
    send_ls(cs, claddrin);
  }
  return 0;
}
/*

  //exit message from child process
  int em;


  //prepare the arguments for call to exec; add one to cmd_argc for cmd itself
  char *arg_arr[cs->cmd_argc+1];
  prep_args(cs, arg_arr);
  
  pid_t spawnpid = -2;
  spawnpid = fork();

  switch(spawnpid)
  {
    case -1:
      perror("Fork in run_fg_child returned error");
      exit(1);
      break;
    case 0:
      //set ridirection 
      if(cs->redir_out > -1)
        redir_stdout(cs);
      if(cs->redir_in > -1)
        redir_stdin(cs);
      //set child mask to guard against SIG_TSTP 
      set_fg_mask(); 
      exec_cmd(cs, arg_arr);
      break;
    default:
      if(DEBUG){fprintf(stderr, "In parent's case; spawnpid: %i\n", spawnpid);}
      spawnpid = waitpid(spawnpid, &em, 0); 
      if (WIFEXITED(em))
      {
        if(SIGDEBUG){fprintf(stderr, "The process (%i) exited normally\n", spawnpid);}
        fge->status = WEXITSTATUS(em);
      }
      else if(WIFSIGNALED(em))
      {
        if(DEBUG){fprintf(stderr, "The process was signalled.\n");}
        fge->signal = WTERMSIG(em);
        fprintf(stderr, "terminated by signal %i\n", fge->signal);
      }
      else
      {
        fprintf(stderr, "Why are we here? Status/Signal: %i\n", fge->signal);
      }
      break;
  }
  free_exec_args(cs->cmd_argc+1, arg_arr); 
  return spawnpid;  
}
*/

