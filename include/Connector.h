#pragma once

#include "Eventloop.h"
#include "HttpHandle.h"

#include <netinet/in.h>
#include <memory>
#include <string>
#include <functional>

class Connector
{
public:
    std::function<void()> rm_call_back_to_acceptor;

public:
    Connector(Eventloop *loop, sockaddr_in addr_, int new_fd, std::string &conn_name);
    void new_message();
    void close_connection();
    std::string get_name();

private:
    std::string conn_name_;
    struct sockaddr_in addr_;

    Eventloop *loop_;
    Channel *channel_;
    HttpHandle *http_handle_;
};