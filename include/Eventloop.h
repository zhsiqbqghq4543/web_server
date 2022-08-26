#pragma once

#include "Epoller.h"
#include "Channel.h"

#include <vector>
#include <functional>
#include <mutex>

class Channel;
class Epoller;

class Eventloop
{
public:
    Eventloop();
    void push_Channel(Channel *channel_);
    void loop();
    void rm_channel(int fd);
    int get_event_fd();
    void do_wake_up_func();
    void push_func(const std::function<void()> &func);
    void wakeup();

    Epoller *epoller_;

private:
    bool runing;
    std::vector<Channel *> active_channels_; // from begin to end
    Channel *current_active_channels_;
    int event_fd_;
    std::mutex mutex_;
    std::vector<std::function<void()>> vec_func_;
};