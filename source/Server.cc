
#include "Server.h"

#include <iostream>
Server::Server(EventloopPool *loop_pool,
               const char *ip,
               const char *port)
    : acceptor_(ip, port, loop_pool->get_main_eventloop()), // socket bind listen
      accept_channel_(loop_pool->get_main_eventloop())
{

  accept_channel_.set_fd(acceptor_.get_fd()); // channel

    // channel to eventloop (epoll)
  loop_pool->get_main_eventloop()->push_Channel(&this->accept_channel_);

  // set channel_read_call_back
  //!!class func  first is this
  this->accept_channel_.set_read_callback(
      std::bind(&Acceptor::new_connection, &this->acceptor_, loop_pool));
}