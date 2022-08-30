#pragma once

#include <iostream>
#include <string>
#include <condition_variable>
#include <mutex>

#include <unistd.h>
#include <fcntl.h>

class LogBack
{
public:
    LogBack();
    void append_to_buffer(std::string &str);
    void thread_loop();

private:
    bool condition_bool_;
    int file_name_num_;
    int file_fd_;
    std::string log_buffer_;
    std::mutex log_mutex_;
    std::condition_variable condition_variable_class_;
};
