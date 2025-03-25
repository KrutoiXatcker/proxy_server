#include "TargetServerConnector.h"
#include "logger.h"
#include <netdb.h>       
#include <cstring>       
#include <sys/socket.h>  
#include <unistd.h>      

SocketWrapper TargetServerConnector::connect(const std::string& host, int port)
{
    struct addrinfo hints, *res;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    if (getaddrinfo(host.c_str(), std::to_string(port).c_str(), &hints, &res) != 0) {
        Logger::error("Failed to resolve host: " + host);
        return SocketWrapper();
    }

    SocketWrapper socket(::socket(res->ai_family, res->ai_socktype, res->ai_protocol));
    if (!socket.isValid()) {
        Logger::error("Failed to create socket for: " + host);
        freeaddrinfo(res);
        return SocketWrapper();
    }

    if (::connect(socket.get(), res->ai_addr, res->ai_addrlen) < 0) {
        Logger::error("Failed to connect to: " + host + ":" + std::to_string(port));
        freeaddrinfo(res);
        return SocketWrapper();
    }

    freeaddrinfo(res);
    return socket;
}