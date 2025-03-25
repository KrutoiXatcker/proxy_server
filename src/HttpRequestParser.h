#ifndef _HTTP_REQUEST_PARSER_H_
#define _HTTP_REQUEST_PARSER_H_

#include <string>
#include <regex>

class HttpRequestParser {
public:
    struct ConnectRequest {
        std::string host;
        int port;
        bool isConnect;
    };
    
    static ConnectRequest parse(const std::string& request);
};

#endif 