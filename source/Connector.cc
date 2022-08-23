
#include "Connector.h"

#include <string.h>
#include <stdio.h>
#include <functional>

#include <iostream>
#include <string>
#include <vector>

Connector::Connector(sockaddr_in addr, std::string &conn_name)
    : addr_(addr), conn_name_(conn_name)
{

    // http_request
    this->http_handle_ = new HttpHandle();
    // buffer
    this->input_buffer_ = new Buffer();
}
Connector::~Connector()
{
    delete this->channel_;
    delete this->http_handle_;
    delete this->input_buffer_;
    std::cout << "\n"
              << conn_name_ << "\tconnector\thas\tbeen\tdestroyed" << std::endl;
}

void Connector::conn_destroy()
{
    std::cout << "destroying...\n";
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
    int recv_size = this->input_buffer_->read_fd(this->channel_->get_fd());
    if (recv_size != 0)
        std::cout << "thread loop address\t" << loop_ << "\trecv\tmessage\t" << recv_size << std::endl;
    if (recv_size == 0)
    {
        std::cout << "thread loop address\t" << loop_ << "\nget close message\n";
        close_connection();

        return;
    }
    if (this->http_handle_->recv_message(this->input_buffer_) == false)
    {
        const char *err = "HTTP/1.1 400 Bad Request\r\n\r\n";
        send(this->channel_->get_fd(), err, sizeof(err), 0); // todo
        // close
        // shutdown()
    }
    if (this->http_handle_->got_all() == true)
    {
        std::cout << "thread loop address\t" << loop_ << "now http message got_all:    \n";
        this->http_handle_->cout_message();
        // request
    }
}

std::string Connector::get_name()
{
    return this->conn_name_;
}

void Connector::add_channel_to_eventloop(Eventloop *loop, int new_fd)
{
    loop_ = loop;
    channel_ = new Channel(loop);
    channel_->set_fd(new_fd);

    loop->push_Channel(channel_);

    // channel_.set_read_callback();
    channel_->set_read_callback(
        std::bind(&Connector::new_message, this));

    std::cout << "new\tConn\tadded\tto\tloop\t" << std::endl;
}