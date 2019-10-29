#!/bin/bash
if [[ $1 == "DEBUG" ]]
then
  gcc -std=gnu11 -Wall -g -pedantic -o chatclient -DDEBUG chatclient.c
elif [[ $1 == "GDB" ]]
then
  gcc -std=gnu11 -g -o chatclient chatclient.c
else
  gcc -std=gnu11 -o chatclient chatclient.c
fi
