
#include "Connector.h"

#include <string.h>
#include <stdio.h>
#include <functional>

#include <iostream>
#include <string>
#include <vector>

Connector::Connector(Eventloop *loop, sockaddr_in addr, int new_fd, std::string &conn_name)
    : addr_(addr), loop_(loop), conn_name_(conn_name)
{
    channel_ = new Channel(loop);
    channel_->set_fd(new_fd);

    loop->push_Channel(channel_);

    // channel_.set_read_callback();
    channel_->set_read_callback(
        std::bind(&Connector::new_message, this));

    // http_request
    this->http_handle_ = new HttpHandle();
}

void Connector::close_connection()
{
    // self io rm map  and  main io rm conn
    this->rm_call_back_to_acceptor();
    // self io
    this->loop_->rm_channel(this->channel_->get_fd());
}

void Connector::new_message()
{
    std::string str;
    str.reserve(1024);
    int recv_size = 1;
    int size = 255;
    while (recv_size > 0)
    {
        std::string tmp_str(size, ' ');
        char *ptr = &*(tmp_str.begin());
        recv_size = recv(this->channel_->get_fd(), ptr, size + 1, 0);
        if (recv_size == 0)
        {
            close_connection();
            return;
        }
        if (recv_size > 0)
            str += std::move(tmp_str);
    }
    std::cout << str;

    if (this->http_handle_->recv_message(str) == false)
    {
        const char *err = "HTTP/1.1 400 Bad Request\r\n\r\n";
        send(this->channel_->get_fd(), err, sizeof(err), 0);
        // close
        // shutdown()
    }
    if (this->http_handle_->got_all() == true)
    {
        std::cout << "\nnow http message got_all:    \n";
        this->http_handle_->cout_message();
        // request
    }
}

std::string Connector::get_name()
{
    return this->conn_name_;
}