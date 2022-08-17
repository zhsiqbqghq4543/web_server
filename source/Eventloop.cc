#pragma once

#include "Eventloop.h"

Eventloop::Eventloop() : epoller_(this),
                         runing(true),
                         current_active_channels_(nullptr)
{
    this->channels_.reserve(1000000);
    this->active_channels_.reserve(1000000);
}

void Eventloop::push_Channel(Channel *Channel)
{
    this->epoller_.push_channel(Channel);
}

void Eventloop::loop()
{
    while (runing)
    {
        active_channels_.clear();

        this->epoller_.epoll(this->active_channels_);

        for (auto channel : this->active_channels_)
        {
            this->current_active_channels_ = channel;
            current_active_channels_; // handing
        }

        current_active_channels_ = nullptr;
    }
}