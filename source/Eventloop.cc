
#include "Eventloop.h"
#include "LogFront.h"

#include <iostream>
#include <sys/eventfd.h>
#include <unistd.h>

Eventloop::Eventloop() : runing(true),
                         current_active_channels_(nullptr)
{
    this->event_fd_ = eventfd(0, EFD_NONBLOCK);

    //will epollctl eventfd
    epoller_ = new Epoller(this);
    LOG_TRACE << "new epoller  fd:\t"+std::to_string(this->epoller_->get_epollfd());

    this->active_channels_.reserve(1000000);
    
    LOG_TRACE << "new event  fd:\t"+std::to_string(this->event_fd_);
}

void Eventloop::push_Channel(Channel *Channel)
{
    this->epoller_->push_channel(Channel);
}
void Eventloop::do_wake_up_func()
{
    uint64_t a = 0;
    read(this->event_fd_, &a, sizeof(a));

    std::vector<std::function<void()>> funcs;

    {
        std::lock_guard<std::mutex> lock_raii(this->mutex_);
        funcs.swap(this->vec_func_);
    }

    for (int i = 0; i < funcs.size(); ++i)
    {
        funcs[i]();
    }
}

void Eventloop::wakeup()
{
    uint64_t a = 1;
    write(this->event_fd_, &a, sizeof(a));
}

void Eventloop::push_func(const std::function<void()> &func)
{
    {
        std::lock_guard<std::mutex> lock_raii(this->mutex_);
        this->vec_func_.push_back(func);
    }

    wakeup();
}

void Eventloop::loop()
{
    while (runing)
    {
        active_channels_.clear();

        this->epoller_->epoll(this->active_channels_);
        for (auto channel : this->active_channels_)
        {
            this->current_active_channels_ = channel;
            current_active_channels_->handle_event(); // handing
        }

        current_active_channels_ = nullptr;
        // delete
        do_wake_up_func();
    }
}

void Eventloop::rm_channel(int fd)
{
    this->epoller_->rm_channel(fd);
}

int Eventloop::get_event_fd()
{
    return this->event_fd_;
}