#pragma once

#include <queue>
#include <unordered_set>
#include <memory>
#include <functional>
#include <sys/timerfd.h>
#include <time.h>
#include <ctime>
#include <string.h>
#include <functional>

#include "Eventloop.h"

class TimeEvent
{
public:
    std::function<void()> timeout_callback_func_;

public:
    TimeEvent(int index, std::time_t out_time);
    void set_callback(std::function<void()> cb);
    int get_index();

private:
    int index_;
    std::time_t out_time_;
};

class Timer
{
private:
    typedef std::pair<std::time_t, std::weak_ptr<TimeEvent>> time_weak_pair;

    class judge_func_
    {
    public:
        bool operator()(time_weak_pair &a, time_weak_pair &b) { return a.first > b.first; };
    };

public:
    Timer(Eventloop *loop);
    int get_timer_fd();
    int add_timeevent(std::time_t timeout_s, std::function<void()> cb);
    void delete_timeevent(int index);
    void time_out_callback();

private:
    Eventloop *loop_; // weak_ptr

    int to_malloc_index_;
    std::time_t channel_out_time_;

    int timer_fd_;
    Channel *timer_channel_;

    std::unordered_map<int, std::shared_ptr<TimeEvent>> timerevent_hash_;
    std::priority_queue<std::pair<std::time_t, std::weak_ptr<TimeEvent>>, std::vector<time_weak_pair>, judge_func_> timerevent_heap_;
};
