
#include "HttpHandle.h"

HttpHandle::HttpHandle() : request_state_(request_line_data)
{
    this->http_request_ = std::make_unique<HttpRequest>();


}

bool HttpHandle::recv_message(int fd)
{
    
}