**Author:**       boettchc
**Class:**        cs372-400-f2019
**Assignment:**   Program 1 
**Date:**         October 29, 2019

### Compiling and Running the C client

The C program of the client can be compiled by running the `compile.sh` script. It will invoke `gcc` 
with a standard flag specifying `gnu11` and no other options aside from a hard-coded output file 
(`chatclient`).

To run the client, specify the hostname or IP and a port: `./chatclient <fqdn_or_ip> <port>`

### Running the Python server

The Python server can be run by simply typing `./chatserve <port>`, where port is a valid port
number above 1023. There is a bash header (`#!/usr/bin/env python3`) at the top of the chatserve
file that will use whatever `python3` interpreter is in your path.

### Notes on Application Behavior
The program fulfills the bare minimum of the requirements: it doesn't deal with handles that are
longer than 10 characters, and will not accomodate messages longer than 513 bytes (500 bytes for the
message, 10 for the longest possible handle, 1 prompt character, 1 space, and a null byte).

Here is the anticipated process for running and communicating:
  1. Run chatserver
  2. Start chatclient (this creates a connection)
  3. Client types a message. It appears, with a handle, on the server-side terminal.
  4. Server types a message. It appears, with a handle, on the client-side terminal.
  
Either client or server type `\quit` and press Enter. This closes the connection, but the server 
remains running and listening.

An interrupt signal (ctrl-c) kills either process.

### Test History
Initially, testing was done locally, using a terminal mulitplexer and the loopback interface. Then
a tunneled connection was tested, with the server-side script running on flip2 and the client
running on the student's machine. Prior to submission, compilation and testing were done on flip2. 
That resulted in several latent bugs being revealed. But debugging with gdb on that platform was inconsistent. 
Final development and testing was moved to os1.engr.oregonstate.edu (please don't tell Ben Brewster). 

