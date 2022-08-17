#pragma once

#include "Channel.h"
#include "Acceptor.h"


class Server
{
public:
    Server(Eventloop *loop, const char *ip, const char *port);
    void new_connection();

private:
    Eventloop *loop_;
    Channel accept_channel_;
    Acceptor acceptor_;
};