#pragma once

#include"Eventloop.h"

Eventloop::Eventloop()
{
    this->channels_.reserve(100000);
    this->active_channels_.reserve(100000);
    this->current_active_channels_=nullptr;
}