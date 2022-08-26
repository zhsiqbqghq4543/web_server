
#include "HttpHandle.h"
#include "Buffer.h"

#include <iostream>

HttpHandle::HttpHandle() : request_state_(request_line_data)
{
    this->http_request_ = std::make_unique<HttpRequest>();
}

void HttpHandle::cout_message()
{
    std::cout << "request message:  ";
    std::cout << "||\ttype :  " << http_request_.get()->get_type();
    std::cout << "||\tversion :  " << http_request_.get()->get_version();
    std::cout << "||\tpath :  " << http_request_.get()->get_path() << std::endl;
}

bool HttpHandle::got_all()
{
    return this->request_state_ == null_data;
}

bool HttpHandle::recv_message(Buffer *input_buffer)
{

    // std::cout << input_buffer->read_index_ << std::endl;
    // std::cout << input_buffer->write_index_ << std::endl;
    // for (auto c : input_buffer->data_vec_)
    // std::cout << c;
    // std::cout << std::endl;
    // std::cout << input_buffer->data_vec_[0] << std::endl;
    // std::cout << input_buffer->data_vec_[input_buffer->read_index_] << std::endl;

    char *line_data = nullptr;

    int line_size = input_buffer->get_one_line(line_data);
    while (line_size != 0)
    {
        std::string recv_str(line_size, 'a');
        for (int i = 0; i < line_size; ++i)
        {
            recv_str[i] = line_data[i];
        }
        //std::cout << recv_str;
        if (this->request_state_ == request_line_data)
        {

            if (http_request_.get()->init_request_line(recv_str) == false)
            {
                return false;
            }
            request_state_ = headers_data;
        }
        else if (this->request_state_ == headers_data)
        {
            if (recv_str == "\r\n")
            {
                request_state_ = body_data;
                continue;
            }
            if (http_request_.get()->push_header_line(recv_str) == false)
            {
                return false;
            }
        }
        line_size = input_buffer->get_one_line(line_data);
    }
    if (this->request_state_ == body_data)
    {
        input_buffer->clear();
        request_state_ = null_data;
    }
    return true;
}

std::string HttpHandle::get_send_data()
{
    return this->http_request_->get_send_data();
}