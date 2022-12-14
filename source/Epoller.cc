
#include "Epoller.h"
#include"LogFront.h"

#include <assert.h>
#include <iostream>
#include <unistd.h>

Epoller::Epoller(Eventloop *loop_) : owner_loop_(loop_)
{
    this->epollfd_ = epoll_create(1);
    this->events_.resize(16); // num of active evetns will twice  per max

    epoll_event event;
    event.data.fd = loop_->get_event_fd();
    event.events = EPOLLIN;
    epoll_ctl(this->epollfd_, EPOLL_CTL_ADD, loop_->get_event_fd(), &event);
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
    //std::cout << "epollfd\t" << epollfd_ << "\twaiting" << std::endl;

    int events_num = epoll_wait(this->epollfd_,
                                &*this->events_.begin(),
                                static_cast<int>(events_.size()),
                                -1);
    //std::cout << "epollfd\t" << epollfd_ << "\tok" << events_num << std::endl;
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
        //std::cout << fd << std::endl;
        if (iterator != this->channel_map_.end())
        {
            Channel *channel_topush = iterator->second;

            channel_topush->set_revents(events_[i].events);
            //std::cout << (events_[i].events & EPOLLOUT) << std::endl;
            active_channels_.push_back(channel_topush);
        }
    }
}

void Epoller::rm_channel(int fd)
{
    this->channel_map_.erase(fd);
    epoll_ctl(this->epollfd_, EPOLL_CTL_DEL, fd, NULL);
    LOG_TRACE<< "delete from epoll : fd\t" +std::to_string(fd)+"\thas\tclosed\t";
}

int Epoller::get_epollfd()
{
    return this->epollfd_;
}