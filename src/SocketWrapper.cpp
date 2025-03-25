#include "SocketWrapper.h"


SocketWrapper::SocketWrapper(int fd) : socket_fd(fd) {}

SocketWrapper::~SocketWrapper() 
{ 
    closeSocket(); 
}

// Реализация перемещения
SocketWrapper::SocketWrapper(SocketWrapper&& other) noexcept 
    : socket_fd(other.socket_fd) {
    other.socket_fd = -1;
}

SocketWrapper& SocketWrapper::operator=(SocketWrapper&& other) noexcept 
{
    if (this != &other) {
        closeSocket();
        socket_fd = other.socket_fd;
        other.socket_fd = -1;
    }
    return *this;
}

int SocketWrapper::get() const 
{ 
    return socket_fd; 
}

bool SocketWrapper::isValid() const 
{ 
    return socket_fd != -1; 
}

void SocketWrapper::closeSocket() 
{
    if (socket_fd != -1) {
        ::close(socket_fd);
        socket_fd = -1;
    }
}

