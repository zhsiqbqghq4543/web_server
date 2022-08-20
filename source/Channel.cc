
#include "Channel.h"

#include <unistd.h>

Channel::Channel(Eventloop *loop_) : fd_(-1), index_(-1), revents_(-1)
{
}
Channel::~Channel()
{
    close(this->fd_);
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

void Channel::set_fd(int fd)
{
    this->fd_ = fd;
}
