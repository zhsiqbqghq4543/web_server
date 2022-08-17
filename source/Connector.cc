
#include "Connector.h"

#include <string.h>
#include <stdio.h>
#include <functional>

Connector::Connector(Eventloop *loop, sockaddr_in addr, int new_fd)
    : addr_(addr), loop_(loop)
{
    printf("dsaaaaaaa\n");
    channel_ = new Channel(loop);
    channel_->set_fd(new_fd);

    loop->push_Channel(channel_);

    // channel_.set_read_callback();
    channel_->set_read_callback(
        std::bind(&Connector::new_message, this));
}

void Connector::new_message()
{
    char get[100];
    get[99]='\0';
    recv(this->channel_->get_fd(), get, sizeof(get), 0);
    
    printf("%s\n", get);
}