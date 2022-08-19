#pragma once

#include "Eventloop.h"
#include "HttpHandle.h"

#include <netinet/in.h>
#include <memory>

class Connector
{
public:
    Connector(Eventloop *loop, sockaddr_in addr_, int new_fd);
    void new_message();

private:
    Eventloop *loop_;
    struct sockaddr_in addr_;

    Channel *channel_;

    HttpHandle *http_handle_;
};