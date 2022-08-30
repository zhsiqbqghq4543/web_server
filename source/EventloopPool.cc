#include "EventloopPool.h"

void EventloopPool::thread_loop(std::shared_ptr<Eventloop> eventloop)
{
    std::cout << "thread\tbegin" << std::endl;
    eventloop->loop();
}

EventloopPool::EventloopPool(Eventloop *main_eventloop, int thread_num)
    : main_eventloop_(main_eventloop)
{
    for (int i = 0; i < thread_num; ++i)
    {
        std::shared_ptr<Eventloop> eventloop = std::make_shared<Eventloop>();
        this->eventloop_list_.push_back(eventloop);

        std::thread thread_tmp(&EventloopPool::thread_loop, this, eventloop);
        thread_tmp.detach();
    }
    list_iterator_ = eventloop_list_.begin();
}

Eventloop *EventloopPool::get_eventloop()
{
    if (eventloop_list_.size() == 0)
    {
        return main_eventloop_;
    }

    if (list_iterator_ == eventloop_list_.end())
    {
        list_iterator_ = eventloop_list_.begin();
    }

    if (list_iterator_->expired())
    {
        std::list<std::weak_ptr<Eventloop>>::iterator to_delete = list_iterator_;
        list_iterator_++;
        eventloop_list_.erase(to_delete);

        return get_eventloop();
    }

    Eventloop *ret = list_iterator_->lock().get();
    list_iterator_++;

    return ret;
}

Eventloop *EventloopPool::get_main_eventloop()
{
    return main_eventloop_;
}