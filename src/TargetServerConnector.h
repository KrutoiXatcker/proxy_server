#ifndef TARGET_SERVER_CONNECTOR_H
#define TARGET_SERVER_CONNECTOR_H

#include "SocketWrapper.h"
#include <string>

class TargetServerConnector {
public:
    static SocketWrapper connect(const std::string& host, int port);
};

#endif 