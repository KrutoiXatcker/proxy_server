#ifndef _LOGGER_H_
#define _LOGGER_H_


#include <iostream>
#include <string>
#include <mutex>


class Logger {
private:
    static std::mutex logMutex; 

public:
    static void log(const std::string& message);

    static void error(const std::string& message);
};


#endif