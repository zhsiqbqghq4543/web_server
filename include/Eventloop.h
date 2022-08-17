#pragma once

#include "Epoller.h"
#include "Channel.h"

#include <vector>

class Channel;
class Epoller;


class Eventloop
{
public:
    Eventloop();
    void push_Channel(Channel *channel_);
    void loop();

private:
    bool runing;
    Epoller *epoller_;
    std::vector<Channel *> channels_;        // get index of channel
    std::vector<Channel *> active_channels_; // from begin to end
    Channel *current_active_channels_;
};