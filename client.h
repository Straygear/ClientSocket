#ifndef CLIENT_H
#define CLIENT_H

#include <iostream>
#include <string>
#include <cstring>

#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <stdio.h>

#define DEFAULT_BUFLEN 4096
#define DEFAULT_PORT "27015"

class CLIENT
{
public:
    CLIENT(char* args[]);
    virtual ~CLIENT();
    virtual int initialiseSocket();
    virtual int connectToServer();
    virtual int sendData(std::string s);
    virtual int recieveData();
    virtual int disconnectClient();
    virtual void setString(std::string g);
    virtual std::string getString();
    virtual bool connected();


private:
    struct addrinfo* result, *ptr, hints;
    SOCKET ConnectSocket;
    int recvbuflen;
    char** arg;
    char recvbuf[DEFAULT_BUFLEN];
    WSADATA wsaData;
    int iResult;
    std::string get;
    bool status;
};

#endif // CLIENT_H
