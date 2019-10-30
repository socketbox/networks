#!/bin/bash
if [[ $1 == "DEBUG" ]]
then
  gcc -std=gnu11 -Wall -ggdb3 -pedantic -o chatclient -DDEBUG1 chatclient.c
elif [[ $1 == "GDB" ]]
then
   gcc -std=gnu11 -ggdb3 -DDEBUG2 -o chatclient chatclient.c
else
  gcc -std=gnu11 -o chatclient chatclient.c
fi
