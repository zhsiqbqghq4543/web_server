#include "LogBack.h"

LogBack::LogBack()
{
    condition_bool_ = false;
    file_name_num_ = 0;
    log_buffer_.reserve(5000);

    std::string name(std::to_string(file_name_num_));
    file_fd_ = open(name.c_str(), O_CREAT | O_RDWR, 00777);
}

void LogBack::append_to_buffer(std::string &str)
{
    std::lock_guard<std::mutex> lock(log_mutex_);
    log_buffer_ += str;
    if (log_buffer_.size() > 4096)
    {
        condition_bool_ = true;
        this->condition_variable_class_.notify_one();
    }
}

void LogBack::thread_loop()
{
    while (true)
    {
        std::string push_str;

        {
            std::unique_lock<std::mutex> thread_lock(log_mutex_); // raii mutex

            condition_variable_class_.wait_for(thread_lock, std::chrono::seconds(8), [this]
                                               { return condition_bool_ == true; });

            push_str.swap(log_buffer_); // mutex lock state
            condition_bool_ = false;    // ok
        }

        // now mutex unlock

        if (lseek(file_fd_, 0, SEEK_CUR) > 1024000)
        {
            std::cout << lseek(file_fd_, 0, SEEK_CUR) << std::endl;
            // todo
            // tell thread to call void LogBack::thread_push_file()

            close(file_fd_);
            file_name_num_++;
            std::string name(std::to_string(file_name_num_));
            file_fd_ = open(name.c_str(), O_CREAT | O_RDWR, 00777);
        }

        write(file_fd_, &*push_str.begin(), push_str.size());
    }
}