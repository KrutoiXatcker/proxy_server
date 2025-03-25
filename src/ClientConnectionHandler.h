#ifndef CLIENT_CONNECTION_HANDLER_H
#define CLIENT_CONNECTION_HANDLER_H

#include "SocketWrapper.h"
#include "HttpRequestParser.h"
#include "TargetServerConnector.h"
#include "DataTunnel.h"



class ClientConnectionHandler {
public:
    static void handle(SocketWrapper clientSocket);


private:

    static void handleConnectRequest(SocketWrapper clientSocket, const std::string& host, int port);

    static void sendConnectResponse(SocketWrapper& clientSocket);
};

#endif 