#pragma once

#include "Eventloop.h"

class Channel
{
public:
    Channel(Eventloop* loop, int fd_);
    ~Channel();
private:
    int events_;
    const int fd_;
    int index_;
};