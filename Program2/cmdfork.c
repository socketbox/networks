#include "ftserver.h"

(exec_cmd(
pid_t run_fg_child(Cmd *cs, Fgexit *fge)
{
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


