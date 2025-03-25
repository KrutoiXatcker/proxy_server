#ifndef DATA_TUNNEL_H
#define DATA_TUNNEL_H

#include "SocketWrapper.h"
#include "logger.h"

class DataTunnel {
public:
    static void start(SocketWrapper& client, SocketWrapper& server);

private:
    static constexpr int BUFFER_SIZE = 4096;
    static bool transferData(SocketWrapper& from, SocketWrapper& to, char* buffer);
};

#endif 