#include "client.h"

CLIENT::CLIENT(char* args[]): result(0), ptr(0), arg(args), iResult(0), get(""), status(false)
{
    ZeroMemory( &hints, sizeof(hints) );
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    ConnectSocket = INVALID_SOCKET;

    recvbuflen = DEFAULT_BUFLEN;
}

CLIENT::~CLIENT(){}

int CLIENT::initialiseSocket(){
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if(iResult != 0){
        std::cout << "WSAstarup failed: " << iResult << std::endl;
        return 1;
    }
    std::cout << "Socket initalised! " << std::endl;
    return iResult;
}

int CLIENT::connectToServer(){
    iResult = getaddrinfo("localhost", DEFAULT_PORT, &hints, &result);
    if(iResult != 0){
        std::cout << "getaddrinfo failed: " << iResult << std::endl;
        WSACleanup();
        return 1;
    }
    //Attempt to connect to the first address returned by the call to getaddrinfo
    for(ptr=result; ptr != NULL; ptr=ptr->ai_next){

    //Create a SOCKET for connecting to server
    ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
    if(ConnectSocket == INVALID_SOCKET){
        std::cout << "Error at socket(): " << WSAGetLastError() << std::endl;
        freeaddrinfo(result);
        WSACleanup();
        return 1;
    }
    //connect to server
    iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
    if(iResult == SOCKET_ERROR){
        closesocket(ConnectSocket);
        std::cout << "deadie spagethi" << std::endl;
        ConnectSocket = INVALID_SOCKET;
        continue;
    }
    break;
}
    freeaddrinfo(result);

    if(ConnectSocket == INVALID_SOCKET){
        std::cout << "Unable to connect to server!" << std::endl;
        WSACleanup();
        return 1;
    }
    std::cout << "Connected!" << std::endl;
    status = true;
    return iResult;
}

int CLIENT::sendData(std::string s) {
    char sendbuf[sizeof(s) + 1];
    s.copy(sendbuf, s.size() + 1);
    sendbuf[s.size()] = '\0';

    iResult = send(ConnectSocket, sendbuf, (int)strlen(sendbuf) + 1, 0);
    if (iResult == SOCKET_ERROR) {
        std::cout << "send failed: " << WSAGetLastError() << std::endl;
        closesocket(ConnectSocket);
        WSACleanup();
        status = false;
        return 1;
    }
    std::cout << "Bytes sent:: " << iResult << std::endl;
}

int CLIENT::recieveData() {
    //Recieve data until the server closes the connection
  //  do {
        ZeroMemory(recvbuf, recvbuflen);
        iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
        if (iResult > 0) {
            std::cout << "Bytes recieved:: " << iResult << std::endl;
            //std::cout << std::string(recvbuf, 0, iResult) << std::endl;
            setString(std::string(recvbuf, 0, iResult));
            return iResult;
        }
        else if (iResult == 0) {
            std::cout << "Connection closed" << std::endl;
            return iResult;
        }
        else {
            std::cout << "recv failed: " << WSAGetLastError() << std::endl;
            return 1;
        }
   // } while (true);
    // return iResult;
}


int CLIENT::disconnectClient(){
    iResult = shutdown(ConnectSocket, SD_SEND);
    if(iResult == SOCKET_ERROR){
        std::cout << "shutdown failed: " << WSAGetLastError() << std::endl;
        closesocket(ConnectSocket);
        WSACleanup();
        return 1;
    }
    std::cout << "Disconnected!" << std::endl;
    status = false;
    return iResult;
}

void CLIENT::setString(std::string g){
    get = g;
}

std::string CLIENT::getString(){
    return get;
}

bool CLIENT::connected(){
    return status;
}
