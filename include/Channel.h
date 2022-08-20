#pragma once

#include "Eventloop.h"

#include <functional>

class Eventloop;

class Channel
{
public:
    Channel(Eventloop *loop);
    ~Channel();
    int get_fd();
    void set_revents(int revents);

    void set_read_callback(std::function<void()> cb);
    void handle_event();
    void set_fd(int fd);

private:
    int revents_;
    int fd_;
    int index_;
    std::function<void()> read_call_back;
};