#include "ProxyServer.h"
#include "logger.h"
#include <sys/socket.h> // socket, bind, listen, accept
#include <netinet/in.h> // sockaddr_in, INADDR_ANY
#include <unistd.h>     // close
#include <utility>      // std::move

ProxyServer::ProxyServer(int port) : port_(port) {}

ProxyServer::~ProxyServer() 
{
    serverSocket_.closeSocket();
}

bool ProxyServer::start() 
{
    serverSocket_ = createServerSocket();
    if (!serverSocket_.isValid()) return false;

    Logger::log("Proxy server started on port " + std::to_string(port_));
    return true;
}

void ProxyServer::run() 
{
    while (true) {
        SocketWrapper clientSocket = acceptClient();
        if (!clientSocket.isValid()) continue;

        std::thread([client = std::move(clientSocket)]() mutable {
            ClientConnectionHandler::handle(std::move(client));
        }).detach();
    }
}

SocketWrapper ProxyServer::createServerSocket() 
{
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port_);

    SocketWrapper socket(::socket(AF_INET, SOCK_STREAM, 0));
    if (!socket.isValid()) {
        Logger::error("Failed to create server socket");
        return SocketWrapper();
    }

    // Установка опции для повторного использования адреса
    int opt = 1;
    if (setsockopt(socket.get(), SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        Logger::error("Failed to set SO_REUSEADDR");
        return SocketWrapper();
    }

    if (bind(socket.get(), (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        Logger::error("Failed to bind socket");
        return SocketWrapper();
    }

    if (listen(socket.get(), SOMAXCONN) < 0) {
        Logger::error("Failed to listen on socket");
        return SocketWrapper();
    }

    return socket;
}

SocketWrapper ProxyServer::acceptClient() 
{
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    
    int client_fd = accept(serverSocket_.get(), (struct sockaddr*)&client_addr, &client_len);
    if (client_fd < 0) {
        Logger::error("Failed to accept client connection");
        return SocketWrapper();
    }
    
    return SocketWrapper(client_fd);
}