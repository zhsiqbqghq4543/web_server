#pragma once

#include "HttpRequest.h"
#include "Buffer.h"

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
    bool recv_message(Buffer *input_buffer);
    bool got_all();
    void cout_message();
    std::string get_send_data();

private:
    RequestState request_state_;
    std::unique_ptr<HttpRequest> http_request_;
};