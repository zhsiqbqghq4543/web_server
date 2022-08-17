#pragma once

#include "Server.h"

Server::Server(Eventloop *loop,
               const char *ip,
               const char *port)
    : acceptor_(ip, port),                      // socket bind listen
      accept_channel_(loop, acceptor_.get_fd()) // channel
{
    // channel to eventloop (epoll)
    loop->push_Channel(&this->accept_channel_);
    // set channel_read_call_back
    //!!class func  first is this
    this->accept_channel_.set_read_callback(
        std::bind(&Acceptor::new_connection, &this->acceptor_, loop));
}