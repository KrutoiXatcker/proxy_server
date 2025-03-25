#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <thread>
#include <vector>
#include <sstream>
#include <regex>
#include <memory>
#include <cstdio>

#include "logger.h"
#include "SocketWrapper.h"
#include "HttpRequestParser.h"
#include "TargetServerConnector.h"
#include "DataTunnel.h"
#include "ClientConnectionHandler.h"
#include "ProxyServer.h"

const int BUFFER_SIZE = 4096;
const int STATIC_PORT = 8080;  

void print_usage(const char* prog_name) {
    std::cerr << "Usage: " << prog_name << " [-l log_file] [-e error_file]\n"
              << "Options:\n"
              << "  -l <file>  Redirect standard output to log file\n"
              << "  -e <file>  Redirect standard error to error file\n";
}

int main(int argc, char* argv[]) {
    std::string log_file, error_file;
    
    int opt;
    while ((opt = getopt(argc, argv, "l:e:")) != -1) {
        switch (opt) {
            case 'l':
                log_file = optarg;
                break;
            case 'e':
                error_file = optarg;
                break;
            default:
                print_usage(argv[0]);
                return 1;
        }
    }
    
    // Проверяем, нет ли лишних аргументов
    if (optind < argc) {
        std::cerr << "Unexpected arguments. Port is static (" << STATIC_PORT << ").\n";
        print_usage(argv[0]);
        return 1;
    }
    

    if (!log_file.empty()) {
        FILE* fptr = freopen(log_file.c_str(), "w", stdout);
        if (fptr == nullptr) {
            std::cerr << "Failed to redirect stdout to " << log_file << "\n";
            return 1;
        }
    }
    

    if (!error_file.empty()) {
        FILE* fptr = freopen(error_file.c_str(), "w", stderr);
        if (fptr == nullptr) {
            std::cerr << "Failed to redirect stderr to " << error_file << "\n";
            return 1;
        }
    }
    
    ProxyServer proxy(STATIC_PORT);
    if (!proxy.start()) {
        return 1;
    }
    
    proxy.run();
    return 0;
}