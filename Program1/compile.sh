#!/bin/bash
if [[ $1 == "DEBUG" ]]
then
  gcc -std=gnu11 -Wall -g -pedantic -o chatclient -DDEBUG chatclient.c
else
  gcc -std=gnu11 -o chatclient chatclient.c
fi
