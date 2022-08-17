#pragma once

#include "Eventloop.h"

#include <functional>

class Channel
{
public:
    Channel(Eventloop *loop, int fd_);
    ~Channel();
    int get_fd();
    void set_revents(int revents);

    void set_read_callback(std::function<void()> cb);
    void handle_event();

private:
    int revents_;
    const int fd_;
    int index_;
    std::function<void()> read_call_back;
};