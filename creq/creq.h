#pragma once

#include <iostream>
#include <string>
#include <map>

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



// START NAMESPACE CREQ // 



namespace creq
{



typedef struct types{
    int family;
    int type;
    int protocol;
} types;

// Sockets //

class Received {
public:
    char* data;
    int result;
    Received(char* data, int result);
};

class Socket{
private:
    SOCKET con = INVALID_SOCKET;
    bool initalized = false;
    bool connected = false;


    void _errorInit(std::string const message);
    bool  _initSocket(types);
    bool _initWin(types);
    void _initMap();

public:
    Socket(int family = AF_INET, int type = SOCK_STREAM, int protocol = IPPROTO_TCP);
    ~Socket();
    bool Active();
    bool Connected();
    std::map<std::string, int> Options();

    void Close();
    bool Connect(std::string host, int port);
    int Send(std::string buf, int flags = 0);
    char* Recv(int buflen, int flags = 0);
    Received RecvDetails(int buflen, int flags = 0);

};



// Response // 
class Response {

};

// Creq class // 

class Creq {    

private:
    bool initalized = false;
    creq::Socket con;


public:
    Creq();
    Response get(std::string url);


};

}
// END NAMESPACE CREQ // 