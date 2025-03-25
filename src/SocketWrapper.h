#ifndef _SOCKET_WRAPPER_H_
#define _SOCKET_WRAPPER_H_


#include <unistd.h>      
#include <sys/socket.h>  


class SocketWrapper {
private:
    int socket_fd;
    
public:
    explicit SocketWrapper(int fd = -1);
    ~SocketWrapper();
    
    // Запрещаем копирование
    SocketWrapper(const SocketWrapper&) = delete;
    SocketWrapper& operator=(const SocketWrapper&) = delete;
    
    // Разрешаем перемещение
    SocketWrapper(SocketWrapper&& other) noexcept;
    SocketWrapper& operator=(SocketWrapper&& other) noexcept;
    
    int get() const;
    bool isValid() const;
    void closeSocket();
};

#endif 