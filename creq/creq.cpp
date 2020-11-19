#ifndef UNICODE
#define UNICODE 1
#endif

#include <iostream>
#include <string>
#include <map>
#include "creq.h"


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

bool replace(std::string& str, const std::string& from, const std::string& to) {
    size_t start_pos = str.find(from);
    if(start_pos == std::string::npos)
        return false;
    str.replace(start_pos, from.length(), to);
    return true;
}


// ----------------------------------- //
// Sockets // ¨
// ----------------------------------- //






// error during initalization (private method)
void creq::Socket::_errorInit(std::string const message){
    std::cout << "Error while initalizing socket object, " << message << std::endl;
}

// initalizer for windows os (private method)
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
    return true;
}


// initalizer (private method)
bool creq::Socket::_initSocket(creq::types _types){
    switch(_os_type){
        case 0: // win 
            return creq::Socket::_initWin(_types);
        case 1: // *nix;
            break;
    }
    return false;
    
}

// Constructor
creq::Socket::Socket(int family, int type, int protocol){
    creq::types _info = {
        family,
        type,
        protocol
    };
    this->initalized = (bool)_initSocket(_info);
    if(!initalized){
        std::cout << "Socket creation failed. " << WSAGetLastError() << std::endl;
    } 
}

// Destructor

creq::Socket::~Socket(){
    if(initalized){
        closesocket(this->con);
    }
}

// Options method 
std::string keys[10] = {"AF_INET", "SOCK_STREAM"};
int values[10] = {AF_INET, SOCK_STREAM};
std::map<std::string, int> options;

std::map<std::string, int> creq::Socket::Options(){
    int i = 0;
    std::map<std::string, int> options;
    for(std::string key : keys){
        options[key] = values[i];
    }
    return options;
}


// Active method
bool creq::Socket::Active(){
    return initalized;
}

creq::Received::Received(char* _data, int _result){
    this->data = _data;
    this->result = _result;
}

// ------------- SOCKET METHODS  ---------------- //

// Close method 
void creq::Socket::Close(){
    this->connected = false;
    closesocket(this->con);
}

// Connect method
bool creq::Socket::Connect(std::string host, int port){
    sockaddr_in details;
    details.sin_family = AF_INET;
    details.sin_addr.s_addr = inet_addr(host.c_str());
    details.sin_port = htons(port);
    int res = connect(this->con, (SOCKADDR *) & details, sizeof (details));
    if(res == SOCKET_ERROR){
        return false;
    }

    this->connected = true;
    
    return true;
}

// Send method
int creq::Socket::Send(std::string buf, int flags){
    if(!connected){
        return 0;
    }
    int res = send(this->con, buf.c_str(), (int)strlen(buf.c_str()), flags);
    if(res == SOCKET_ERROR){
        std::cout << "Failed sending data to host" << std::endl;
    }
    return res;
}

// Recv method
char* creq::Socket::Recv(int buflen, int flags){
    if(!connected){
        return 0;
    }
    char *recvbuf = new char[buflen];
    int res = recv(this->con, recvbuf, buflen, flags);
    return recvbuf;
}
creq::Received creq::Socket::RecvDetails(int buflen, int flags){
    char *recvbuf = new char[buflen];
    int res = recv(this->con, recvbuf, buflen, flags); 
    return creq::Received(recvbuf, res);
}


// ----------------------------------- //
// Main creq // 
// ----------------------------------- //



// constructor
creq::Creq::Creq(){
    this->con = creq::Socket(
        AF_INET,
        SOCK_STREAM,
        IPPROTO_TCP
    );
    if(!con.Active()){
        std::cout << "Failed creating creq object" << std::endl;
    }
}

creq::Response creq::Creq::get(std::string url){
    int port = PORT_HTTPS;
    if(url.length() > 5){
        if((char)url[4] != 's' && (char)url[4] != 'S'){
            port = PORT_HTTP;
        }
    }

    std::cout << url << "|" << port << std::endl;
    if(!this->con.Connect(url, port)){
        std::cout << "Failed to connect to host" << std::endl;
        return creq::Response();
    } 
    
    std::string data = "GET HTTP/1.1\r\n"
    "Host: $[Host]$" 
    "\r\n\r\n";
    std::string keys[10] = {"$[Host]$"};
    std::string values[10];
    int i = 0;
    for(std::string key : keys){
        replace(data, key, values[i]);
    }

    this->con.Send(data);
}


// remember -lws2_32 when compiling



