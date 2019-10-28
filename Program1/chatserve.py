'''
Author:       boettchc
Class:        cs372-400-f2019
Assignment:   Program 1 
Date:         October 28, 2019

Much of this code was taken from Section 2.7 of Kurose and Ross.
Anything not attributable to that source is an amalgam of knowledge gleened from
the official Python documentation at docs.python.org and D. Beazley's "Python
Cookbook," published by O'Reilly.
'''

from socket import *
import argparse
import signal
import sys
import re

#regex for checking to see if the client wants to quit
QUIT=br'^.*\\quit'
DEBUG=False
HANDLE='boettchc> '

'''
pre:  a SIGINT has been received by the program
in:   a signal object and a frame object
out:  n/a
post: application exits with status 0
'''
def trap_sigint(sig, frame):
  print('SIGINT received. Exiting.');
  frame.clear() 
  sys.exit(0);

'''
pre:  n/a
in:   n/a
out:  a Namespace object, as created by the ArgumentParser
post: global var DEBUG set to true if arguments included '-d'
note: the '-d' argument is for internal use only and is not shown to the user in usage
'''
def parse_args():
  global DEBUG
  parser = argparse.ArgumentParser(description='A simple chatserver.')
  parser.add_argument('-d', action='store_true', help=argparse.SUPPRESS) 
  parser.add_argument('port', type=int, nargs=1, help='the port that the chatserver should listen on') 
  args = parser.parse_args() 
  if args.d:
    DEBUG=True
    print(args)
    print(vars(args))
  return args

'''
pre:  connection established with client and 
in:   a byte sequence received by the client, a regular expression, and an open socket  
out:  an integer value indicating if communication is to proceed
post: if regex matches on msg, cx is closed
'''
def parse_msg(msg, regex, cx):
  retval = False 
  pattern = re.compile(regex)
  if pattern.match(msg):
    #close cx
    print("Quitting...Bye!\n")
    cx.shutdown(SHUT_RDWR) 
    cx.close()
  else:
    retval = True
  return retval

'''
pre:  n/a 
in:   a port supplied by the user which the server will listen on
out:  n/a
post: server listening on port parameter
'''
def start_server(port):
  if DEBUG:
    print('Starting server on port ', port)
  serverSocket = socket(AF_INET,SOCK_STREAM)
  serverSocket.bind(('', port))
  serverSocket.listen(1)
  while True:
    connectionSocket, addr = serverSocket.accept()
    msgin = connectionSocket.recv(500)
    if parse_msg(msgin, QUIT, connectionSocket):
      print(msgin.decode('utf-8'))
      msgout = input(HANDLE)
      enc_msgout = (HANDLE+msgout).encode('ascii') 
      connectionSocket.send(enc_msgout)
      
def main():
  #register the signal handler for SIGINT
  signal.signal(signal.SIGINT, trap_sigint)

  #parse cmdline
  args = parse_args()
  if DEBUG:
    print('port arg: ', args.port[0]);
 
  start_server(args.port[0])

if __name__ == "__main__":
      main()
