#!/usr/bin/bash
rm -f ftserver
DFLAG=""

if [[ "$1" == '-t' ]]
then
	echo "CTAGS"
	ctags -R ./*
fi

if [[ "$1" == '-g' ]]
then
	echo "DEBUG"
	DFLAG="-DDEBUG"
	rm -f ./*.o ftserver	
	gcc -std=gnu11 $DFLAG -Og -g3 -Wall -pedantic -I. -c cmdfork.c
	gcc -std=gnu11 $DFLAG -Og -g3 -Wall -pedantic -I. -o ftserver cmdfork.o ftserver.c
fi
