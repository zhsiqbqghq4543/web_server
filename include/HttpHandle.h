#pragma once

#include "HttpRequest.h"

#include <memory>

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
    bool recv_message(int fd);

private:
    RequestState request_state_;
    std::unique_ptr<HttpRequest> http_request_;
};