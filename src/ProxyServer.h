#ifndef PROXY_SERVER_H
#define PROXY_SERVER_H

#include "SocketWrapper.h"
#include "ClientConnectionHandler.h"
#include <thread>
#include <string>

class ProxyServer {
public:
    explicit ProxyServer(int port);
    ~ProxyServer();
    
    bool start();
    void run();

private:
    int port_;
    SocketWrapper serverSocket_;

    SocketWrapper createServerSocket();
    SocketWrapper acceptClient();
};

#endif 