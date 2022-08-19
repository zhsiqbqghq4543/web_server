
#include "HttpHandle.h"

#include <iostream>

HttpHandle::HttpHandle() : request_state_(request_line_data)
{
    this->http_request_ = std::make_unique<HttpRequest>();
}

void HttpHandle::cout_message()
{
    std::cout << "request message:  ";
    std::cout << "\ntype :  " << http_request_.get()->get_type();
    std::cout << "\nversion :  " << http_request_.get()->get_version();
    std::cout << "\npath :  " << http_request_.get()->get_path() << std::endl;
}

bool HttpHandle::got_all()
{
    return this->request_state_ == null_data;
}

bool HttpHandle::recv_message(std::string &recv_str)
{
    size_t ptr = 0;
    size_t size = recv_str.size();

    while (ptr < size)
    {
        if (this->request_state_ == request_line_data)
        {
            size_t new_ptr = recv_str.find("\r\n");
            if (new_ptr == std::string::npos)
            {
                ptr = size;
                break;
            }
            new_ptr++;

            if (http_request_.get()->init_request_line(recv_str, 0, new_ptr) == false)
            {
                ptr = size;
                break;
            }

            ptr = new_ptr + 1;
            request_state_ = headers_data;
        }
        else if (this->request_state_ == headers_data)
        {
            if (recv_str[ptr] == '\r' && recv_str[ptr + 1] == '\n')
            {
                request_state_ = body_data;
                ptr += 2;
                if (ptr == size) // only get //TODO
                    this->request_state_ = null_data;

                continue;
            }

            size_t new_ptr = recv_str.find("\r\n", ptr);
            if (new_ptr == std::string::npos)
            {
                ptr = size;
                break;
            }
            new_ptr++;

            if (http_request_.get()->push_header_line(recv_str, ptr, new_ptr) == false)
            {
                ptr = size;
                break;
            }

            ptr = new_ptr + 1;
        }
        else if (this->request_state_ == body_data)
        {
            http_request_.get()->push_body_line(recv_str, ptr);
            this->request_state_ = null_data;
            ptr = size;
        }
    }
    if (ptr < size)
        return true;
    return false;
}