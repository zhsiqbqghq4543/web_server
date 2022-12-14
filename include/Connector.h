#pragma once

#include "Eventloop.h"
#include "HttpHandle.h"
#include "Buffer.h"

#include <netinet/in.h>
#include <memory>
#include <string>
#include <functional>

class Connector:public std::enable_shared_from_this<Connector>
{
public:
    std::function<void()> rm_call_back_to_acceptor;

public:
    Connector(sockaddr_in addr_, std::string &conn_name);
    ~Connector();
    void new_message();
    void close_connection();
    void close_connection_and_timer();
    std::string get_name();
    void conn_destroy();
    void add_channel_to_eventloop(Eventloop *loop, int new_fd);
    void send_message();
    void timer_out();
private:
    int timer_index_;
    std::string conn_name_;
    struct sockaddr_in addr_;

    Eventloop *loop_;
    Channel *channel_;
    HttpHandle *http_handle_;
    Buffer *input_buffer_;
    Buffer *output_buffer_;
};