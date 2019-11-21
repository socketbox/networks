from socket import *

HANDLE="HocusPocus> "

while True:
    #serverName = '128.193.54.182'
    serverName = '127.0.0.1'
    #serverPort = 12000
    serverPort = 2503
    clientSocket = socket(AF_INET, SOCK_STREAM)
    clientSocket.connect((serverName,serverPort))

    sentence = input(HANDLE)
    print("You provided: " + sentence)
    enc_sent = str.encode(HANDLE+sentence);
    clientSocket.send(enc_sent)
    modifiedSentence = clientSocket.recv(500)
    print(modifiedSentence)
    clientSocket.close()


