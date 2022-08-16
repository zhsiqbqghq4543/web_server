#pragma once

#include <vector>
#include "Epoller.h"
// Class Epoller;
class Channel;

class Eventloop
{
public:
    Eventloop();
    ~Eventloop();
    void push_Channel(Channel *channel_);

private:
    Epoller epoller_;
    std::vector<Channel *> channels_;        // get index of channel
    std::vector<Channel *> active_channels_; // from begin to end
    Channel *current_active_channels_;
};