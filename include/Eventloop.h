#pragma once

#include "Epoller.h"
#include "Channel.h"
#include "Timer.h"

#include <vector>
#include <functional>
#include <mutex>

class Channel;
class Epoller;
class Timer;

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
    int get_epollfd();
    int add_timer(std::time_t timeout_s, std::function<void()> cb);
    void delete_timer(int index);
private:
    Epoller *epoller_;
    Timer *loop_timer_;
    bool runing;
    std::vector<Channel *> active_channels_; // from begin to end
    Channel *current_active_channels_;
    int event_fd_;
    std::mutex mutex_;
    std::vector<std::function<void()>> vec_func_;
};