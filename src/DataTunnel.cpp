#include "DataTunnel.h"
#include <sys/select.h>  
#include <algorithm>     

void DataTunnel::start(SocketWrapper& client, SocketWrapper& server) 
{
    fd_set read_fds;
    char buffer[BUFFER_SIZE];
    
    while (true) {
        FD_ZERO(&read_fds);
        FD_SET(client.get(), &read_fds);
        FD_SET(server.get(), &read_fds);

        int max_fd = std::max(client.get(), server.get());
        if (select(max_fd + 1, &read_fds, nullptr, nullptr, nullptr) < 0) {
            Logger::error("Select error");
            break;
        }

        if (FD_ISSET(client.get(), &read_fds)) {
            if (!transferData(client, server, buffer)) break;
        }

        if (FD_ISSET(server.get(), &read_fds)) {
            if (!transferData(server, client, buffer)) break;
        }
    }
}

bool DataTunnel::transferData(SocketWrapper& from, SocketWrapper& to, char* buffer) 
{
    ssize_t bytes_received = recv(from.get(), buffer, BUFFER_SIZE, 0);
    if (bytes_received <= 0) return false;
    
    if (send(to.get(), buffer, bytes_received, 0) < 0) {
        Logger::error("Failed to send data");
        return false;
    }
    return true;
}