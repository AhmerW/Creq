#ifndef UNICODE
#define UNICODE 1
#endif

#include <iostream>
#include <string>
#include "creq.hpp"



#if defined(_WIN32) || defined(_WIN64)
    #define _os_type 0
    #pragma comment(lib, "Ws2_32.lib")

    #include <winsock2.h>
    #include <ws2tcpip.h>
    #include <stdio.h>
#else
    #define _os_type 1
    typedef int SOCKET;
    #include <sys/socket.h>
    #include <arpa/inet.h>
    #include <netdb.h>
    #include <unistd.h>
    #include <fcntl.h>
#endif

#define DEFAULT_BUFLEN 512
#define PORT_HTTP 80
#define PORT_HTTPS 443



// Sockets // 

void creq::Socket::_errorInit(std::string const message){
    std::cout << "Error while initalizing socket object, " << message << std::endl;
}

bool creq::Socket::_initWin(creq::types info){
   
    WSADATA wsaData;
    int iResult = 0;
    iResult = WSAStartup(MAKEWORD(1, 1), &wsaData);
    if(iResult != 0){
        _errorInit("WSAStartup failed.");
        return false;
    }
    this->con = socket(info.family, info.type, info.protocol);
    if(this->con == INVALID_SOCKET){
        WSACleanup();
        return false;
    }
    WSACleanup();
    return true;
}

bool creq::Socket::_initSocket(creq::types _types){
    switch(_os_type){
        case 0: // win 
            return creq::Socket::_initWin(_types);
        case 1: // *nix;
            break;
    }
    return false;
    
}

creq::Socket::Socket(int family = AF_INET, int type = SOCK_STREAM, int protocol = 0){
    creq::types _info = {
        family,
        type,
        protocol
    };
    this->initalized = _initSocket(_info);
    if(!initalized){
        std::cout << "Socket creation failed. " << WSAGetLastError() << std::endl;
    } 
}

creq::Socket::~Socket(){
    if(initalized){
        closesocket(this->con);
    }
}

bool creq::Socket::active(){
    return initalized;
}

bool creq::Socket::Connect(std::string host, int port){
    sockaddr_in details;
    details.sin_family = AF_INET;
    details.sin_addr.s_addr = inet_addr(host.c_str());
    details.sin_port = htons(port);
    std::cout << details.sin_addr.s_addr << "|" << details.sin_port << std::endl;
    int res = connect(this->con, (SOCKADDR *) & details, sizeof (details));
    std::cout << (this->con == INVALID_SOCKET) << std::endl;
    if(res == SOCKET_ERROR){
        return false;
    }


    return true;
}

// Main creq // 



creq::Creq::Creq(){
    this->socket = creq::Socket(
        AF_INET,
        SOCK_STREAM,
        IPPROTO_TCP
    );
    if(!socket.active()){
        std::cout << "Failed creating creq object" << std::endl;
    }
}



// remember -lws2_32 when compiling


int main(int argc, char* argv[]){

    using namespace creq;
    Socket socket = Socket();
    std::cout << socket.active() << std::endl;
    std::cout << socket.Connect("127.0.0.1", 9991) << std::endl;

    return 0;
}


