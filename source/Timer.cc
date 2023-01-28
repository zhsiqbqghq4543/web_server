#include "Timer.h"

TimeEvent::TimeEvent(int index, std::time_t out_time) : index_(index), out_time_(out_time) {}

void TimeEvent::set_callback(std::function<void()> cb)
{
    timeout_callback_func_ = std::move(cb);
}

int TimeEvent::get_index()
{
    return index_;
}

Timer::Timer(Eventloop *loop)
    : to_malloc_index_(0), loop_(loop), channel_out_time_(INT32_MAX), timer_fd_(0)
{
    timer_fd_ = timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK);
    struct itimerspec timebuf;
    memset(&timebuf, 0, sizeof(timebuf));
    timerfd_settime(timer_fd_, TFD_TIMER_ABSTIME, &timebuf, NULL);

    timer_channel_ = new Channel(loop); // it must in its io thread
    timer_channel_->set_fd(timer_fd_);

    timer_channel_->set_read_callback(
        std::bind(&Timer::time_out_callback, this));

    loop->push_Channel(timer_channel_);
}

int Timer::get_timer_fd()
{
    return timer_fd_;
}

int Timer::add_timeevent(std::time_t timeout_s, std::function<void()> cb)
{
    std::time_t time_now = std::time(nullptr);
    time_now += timeout_s;

    int index = ++to_malloc_index_;//!!!!!!!!!!!!!!!!!!!!!todo,bug,
    std::shared_ptr<TimeEvent> timeevent_share_ = std::make_shared<TimeEvent>(index, time_now);
    timeevent_share_->set_callback(cb);

    timerevent_hash_.insert({index, timeevent_share_});
    timerevent_heap_.push({time_now, timeevent_share_});

    if (time_now <= channel_out_time_ || timerevent_heap_.size() == 1)
    {
        channel_out_time_ = timeout_s;
        struct itimerspec timebuf;
        memset(&timebuf, 0, sizeof(timebuf));
        timebuf.it_value = {timeout_s, 0};
        timerfd_settime(timer_fd_, 0, &timebuf, NULL);
    }

    return index;
}

void Timer::delete_timeevent(int index)
{
    timerevent_hash_.erase(index);
}

void Timer::time_out_callback()
{
    std::time_t time_now = std::time(nullptr);

    while ((timerevent_heap_.empty() == false) && (timerevent_heap_.top().first <= time_now))
    {
        if (timerevent_heap_.top().second.expired())
        {
            timerevent_heap_.pop();
            continue;
        }

        timerevent_heap_.top().second.lock()->timeout_callback_func_();
        int index = timerevent_heap_.top().second.lock()->get_index();
        timerevent_hash_.erase(index);
        timerevent_heap_.pop();
    }
    if (timerevent_heap_.empty() == false)
    {
        struct itimerspec timebuf;
        memset(&timebuf, 0, sizeof(timebuf));
        timebuf.it_value = {timerevent_heap_.top().first - time_now, 0};
        timerfd_settime(timer_fd_, 0, &timebuf, NULL);
    }
    else
    {
        struct itimerspec timebuf;
        memset(&timebuf, 0, sizeof(timebuf));
        timerfd_settime(timer_fd_, 0, &timebuf, NULL);
    }
}
