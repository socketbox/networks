#!/usr/bin/bash
rm -f ftserver

if [[ "$1" == '-g' ]]
then
	echo "DEBUG"
	gcc -std=gnu11 -Og -g3 -Wall -pedantic -o ftserver ftserver.c
fi
