#include "ClientConnectionHandler.h"
#include "logger.h"
#include <cstring>      
#include <unistd.h>     
#include <sys/socket.h> 


const int BUFFER_SIZE = 4096;


void ClientConnectionHandler::handle(SocketWrapper clientSocket) {
    char buffer[BUFFER_SIZE];
    ssize_t bytes_received = recv(clientSocket.get(), buffer, BUFFER_SIZE, 0);
    if (bytes_received < 0) {
        Logger::error("Failed to read from client");
        return;
    }

    std::string request(buffer, bytes_received);
    Logger::log("Request received:\n" + request);

    auto parsed = HttpRequestParser::parse(request);
    if (parsed.isConnect) {
        handleConnectRequest(std::move(clientSocket), parsed.host, parsed.port);
    }
}

void ClientConnectionHandler::handleConnectRequest(SocketWrapper clientSocket, const std::string& host, int port) 
{
    SocketWrapper serverSocket = TargetServerConnector::connect(host, port);
    if (!serverSocket.isValid()) return;

    sendConnectResponse(clientSocket);
    DataTunnel::start(clientSocket, serverSocket);
}

void ClientConnectionHandler::sendConnectResponse(SocketWrapper& clientSocket) 
{
    const char* response = "HTTP/1.1 200 Connection Established\r\n\r\n";
    if (send(clientSocket.get(), response, strlen(response), 0) < 0) {
        Logger::error("Failed to send CONNECT response");
    }
}