#pragma once

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <assert.h>

#include "Server.h"

class Server::new_conn_cb
{
public:
    void operator()(int server_fd)
    {
        struct sockaddr_in client;
        socklen_t client_addrlength = sizeof(client);

        int new_fd = accept(server_fd,
                            reinterpret_cast<struct sockaddr *>(&client),
                            &client_addrlength);
        assert(new_fd>0);
        
        //new connection new channel new sockaddr_in
    }
};

Server::Server(Eventloop *loop, int fd) : accept_channel_(loop, fd)
{
    loop->push_Channel(&this->accept_channel_);

    {
        new_conn_cb read_cb;
        this->accept_channel_.set_read_callback(read_cb);
    }
}