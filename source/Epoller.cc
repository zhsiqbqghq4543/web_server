#pragma once

#include "Epoller.h"

#include <assert.h>

Epoller::Epoller(Eventloop *loop_) : owner_loop_(loop_)
{
    this->epollfd_ = epoll_create(1);
    this->events_.resize(16); // num of active evetns will twice  per max
}

void Epoller::push_channel(Channel *Channel)
{
    int fd = Channel->get_fd();
    this->channel_map_[fd] = Channel;

    epoll_event event;
    event.data.fd = fd;
    event.events = EPOLLIN;
    // LTmod
    // EPOLLIN|EPOLLET

    epoll_ctl(this->epollfd_, EPOLL_CTL_ADD, fd, &event);
}

void Epoller::epoll(std::vector<Channel *> &avtive_channels_)
{

    int events_num = epoll_wait(this->epollfd_,
                                &this->events_.front(),
                                static_cast<int>(events_.size()),
                                -1);
    assert(events_num >= 0);
    if (events_num > 0)
    {
        this->fill_active_channels(events_num, avtive_channels_);
        if (events_num == this->events_.size())
        {
            events_.resize(events_.size() * 2);
        }
    }
}

void Epoller::fill_active_channels(int event_num_,
                                   std::vector<Channel *> &active_channels_)
{
    for (int i = 0; i < event_num_; ++i)
    {
        int fd = this->events_[i].data.fd;

        auto iterator = this->channel_map_.find(fd);
        if (iterator != this->channel_map_.end())
        {
            Channel *channel_topush = iterator->second;

            channel_topush->set_revents(events_[i].events);
            active_channels_.push_back(channel_topush);
        }
    }
}
