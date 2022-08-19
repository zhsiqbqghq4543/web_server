#pragma once

#include "HttpRequest.h"

#include <memory>
#include <string>

class HttpRequest;

class HttpHandle
{
public:
    enum RequestState
    {
        request_line_data,
        headers_data,
        body_data,
        null_data
    };

    HttpHandle();
    bool recv_message(std::string &recv_str);
    bool got_all();
    void cout_message();
private:
    RequestState request_state_;
    std::unique_ptr<HttpRequest> http_request_;
};