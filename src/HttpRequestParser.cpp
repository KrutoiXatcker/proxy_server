#include "HttpRequestParser.h"

HttpRequestParser::ConnectRequest HttpRequestParser::parse(const std::string& request) 
{
    ConnectRequest result{};
    std::regex connect_regex(R"(CONNECT\s+([^\s:]+):(\d+))");
    std::smatch match;

    if (std::regex_search(request, match, connect_regex)) {
        result.host = match[1].str();
        result.port = std::stoi(match[2].str());
        result.isConnect = true;
    }
    
    return result;
}