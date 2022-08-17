#pragma once

#include "Connector.h"

#include <string.h>
#include <stdio.h>
#include <functional>

Connector::Connector(Eventloop *loop, sockaddr_in addr, int new_fd)
    : channel_(loop, new_fd), addr_(addr), loop_(loop)
{

    loop->push_Channel(&channel_);

    // channel_.set_read_callback();
    channel_.set_read_callback(
        std::bind(&Connector::new_message, this));
}

void Connector::new_message()
{
    char *get = new char(100);
    memset(get, '\0', 100);
    recv(this->channel_.get_fd(), get, 99, 0);
    printf("%s\n", get);
}