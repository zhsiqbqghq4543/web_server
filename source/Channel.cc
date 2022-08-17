#pragma once

#include "Channel.h"

Channel::Channel(Eventloop *loop_, int fd) : fd_(fd)
{
    this->index_ = -1;
    this->revents_ = -1;
}

int Channel::get_fd()
{
    return this->fd_;
}

void Channel::set_revents(int revents)
{
    this->revents_ = revents;
}

void Channel::set_read_callback(std::function<void()> cb)
{
    this->read_call_back = std::move(cb);
}

void Channel::handle_event()
{
    if (revents_ && EPOLLIN)
    {
        this->read_call_back();
    }
    return;
}
