#pragma once

#include <iostream>
#include <string>

#if defined(_WIN32) || defined(_WIN64)
    #define _os_type 0
    #pragma comment(lib, "Ws2_32.lib")

    #include <winsock2.h>
    #include <stdlib.h>
    #include <WS2tcpip.h>
#else
    #define _os_type 1
    #include <sys/socket.h>
#endif

#define DEFAULT_BUFLEN 512
#define PORT_HTTP 80
#define PORT_HTTPS 443

namespace creq{

typedef struct types{
    int family;
    int type;
    int protocol;
} types;

// Sockets //

class Socket{
private:
    SOCKET con = INVALID_SOCKET;
    bool initalized = false;

    void _errorInit(std::string const message);
    bool  _initSocket(types);
    bool _initWin(types);

public:
    Socket(int family, int type, int protocol);
    ~Socket();
    bool active();
    bool Connect(std::string host, int port);

};

// Response // 
class Response {

};

// Creq class // 

class Creq {    

private:
    bool initalized = false;
    creq::Socket socket;


public:
    Creq();
    Response get(std::string url);


};
}