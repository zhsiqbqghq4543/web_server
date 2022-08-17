#pragma once

#include "Eventloop.h"

#include <vector>
#include <map>
#include <sys/epoll.h>

class Eventloop;
class Channel;

class Epoller
{
public:
    Epoller(Eventloop *loop_);
    void push_channel(Channel *Channel);
    void epoll(std::vector<Channel *> &avtive_channels_);
    void fill_active_channels(int event_num_,
                              std::vector<Channel *> &active_channels_);

private:
    Eventloop *owner_loop_; // owner
    int epollfd_;
    std::vector<struct epoll_event> events_; // active event
    std::map<int, Channel *> channel_map_;   //<fd,Channel*>
};