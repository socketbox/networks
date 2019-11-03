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

If the python server is terminated suddenly and then relaunched with the same set of arguments as
previously, it will attempt to reuse it.

### Notes on Application Behavior
The program fulfills the bare minimum of the requirements: it doesn't deal with handles that are
longer than 10 characters, and will not accomodate messages longer than 513 bytes (500 bytes for the
message body, 10 for the longest possible handle, 1 prompt character, 1 space, and a null byte).

The anticipated process for communicating between client and server is that which is described in 
the assignment sepcification. To wit, the server is started, the client solicits a handle from the 
user, the user types a message, hits enter and both handle and message are sent to the server. The
server notes the handle and prompts the server-side user for a message, which they send by pressing
Enter. Wash, rinse, repeat.

A message starting with '\quit' on the client side will terminate the application. The same message
on the server will break the connection, but keep the server running; Ctrl-C will kill it.

### Testing
Initial testing was done locally, in the development environment, using the loopback interface. 
Further testing was done on flip2, again using the loopback interface. Final testing was done via 
an ssh tunnel from the development environment to flip2.
