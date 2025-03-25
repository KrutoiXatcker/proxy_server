#include "logger.h"

// Инициализация статического мьютекса
std::mutex Logger::logMutex;

void Logger::log(const std::string& message) 
{
    std::lock_guard<std::mutex> lock(logMutex);
    std::cout << "[LOG] " << message << std::endl;
}

void Logger::error(const std::string& message) 
{
    std::lock_guard<std::mutex> lock(logMutex);
    std::cerr << "[ERROR] " << message << std::endl;
}