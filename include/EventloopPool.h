#pragma once

#include "Eventloop.h"

#include <list>
#include <thread>
#include <memory>
#include <functional>
#include <iostream>

class EventloopPool
{
private:
    void thread_loop(std::shared_ptr<Eventloop> eventloop);

public:
    EventloopPool(Eventloop *main_eventloop, int thread_num);
    Eventloop *get_eventloop();
    Eventloop *get_main_eventloop();

private:
    Eventloop *main_eventloop_;
    std::list<std::weak_ptr<Eventloop>>::iterator list_iterator_;
    std::list<std::weak_ptr<Eventloop>> eventloop_list_;
};