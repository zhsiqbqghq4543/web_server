#pragma once

#include <functional>

#include "Eventloop.h"

class Channel
{
public:
    Channel(Eventloop *loop, int fd_);
    ~Channel();
    int get_fd();
    void set_revents(int revents);

    void set_read_callback(std::function<void(int)> cb);
    void handle_event();

private:
    int revents_;
    const int fd_;
    int index_;
    std::function<void(int)> read_call_back;
};