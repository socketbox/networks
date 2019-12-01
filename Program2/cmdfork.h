#ifndef CMDFORK_H
#define CMDFORK_H

#include "ftserver.h"

#define NO_FILE_ERR			-11

int execute_cmd(Cmd cs, struct sockaddr *inetaddr);

#endif
