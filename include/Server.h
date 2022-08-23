#pragma once

#include "Channel.h"
#include "Acceptor.h"
#include"EventloopPool.h"

class Server
{
public:
    Server(EventloopPool *loop_pool, const char *ip, const char *port);
    void new_connection();

private:
    Eventloop *loop_;//own
    Channel accept_channel_;
    Acceptor acceptor_;
};