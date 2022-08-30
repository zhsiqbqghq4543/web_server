#include "LogFront.h"

#include <time.h>
#include <iostream>

namespace log_global
{
    void output(std::string &str)
    {
        std::cout << str;
    }
    int g_logLevel = 0;
    void (*func_ptr)(std::string &str) = output;

}

// LogFrontAbstract LogFrontAbstract LogFrontAbstract LogFrontAbstract LogFrontAbstract
LogFrontAbstract::LogFrontAbstract(std::string file_name,
                                   int line_num,
                                   LogLevel level,
                                   std::string func_name)
{

    log_special_ = std::make_unique<LogFrontSpecial>(file_name, line_num, get_level_str(level), level);
}

LogFrontAbstract::~LogFrontAbstract()
{
    log_special_->finish();

    log_global::func_ptr(stream().get_buffer());
}

LogStream &LogFrontAbstract::stream()
{
    return *(this->log_special_->stream_.get());
}

std::string LogFrontAbstract::get_level_str(LogLevel level)
{
    switch (level)
    {
    case TRACE:
    {
        return "TRACE";
    }
    case DEBUG:
    {
        return "DEBUG";
    }
    case INFO:
    {
        return "INFO";
    }
    case WARN:
    {
        return "WARN";
    }
    case ERROR:
    {
        return "ERROR";
    }
    case FATAL:
    {
        return "FATAL";
    }
    default:
        return "NULL";
    }
}
void LogFrontAbstract::set_log_level(LogLevel level)
{
    // todo
}

// LogFrontAbstract LogFrontAbstract LogFrontAbstract LogFrontAbstract LogFrontAbstract

// LogFrontSpecial LogFrontSpecial LogFrontSpecial LogFrontSpecial LogFrontSpecial
LogFrontSpecial::LogFrontSpecial(std::string &file_name,
                                 int line_num,
                                 std::string level,
                                 int level_num)
    : level_num_(level_num), level_(level), line_num_(line_num)
{

    file_name_.swap(file_name);
    stream_ = std::make_unique<LogStream>();
    stream_->operator<<(level_ + ' ');
}

int LogFrontSpecial::get_level_num()
{
    return this->level_num_;
}

void LogFrontSpecial::finish()
{
    stream_->operator<<("\t-" + file_name_ + ':' + std::to_string(line_num_) + '\n');
}

// LogFrontSpecial LogFrontSpecial LogFrontSpecial LogFrontSpecial LogFrontSpecial

// LogStream LogStream LogStream LogStream LogStream LogStream LogStream LogStream
LogStream::LogStream()
{
    this->buffer_.reserve(128);
    buffer_size_max_ = 4096;

    time_t time_now;
    time(&time_now);
    tm *struct_time_now = localtime(&time_now);

    buffer_ += std::to_string(1900 + struct_time_now->tm_year) + '/'; // year
    buffer_ += std::to_string(1 + struct_time_now->tm_mon) + '/';     // mouth
    buffer_ += std::to_string(struct_time_now->tm_mday) + ' ';        // day
    buffer_ += std::to_string(struct_time_now->tm_hour) + ':';        // hour
    buffer_ += std::to_string(struct_time_now->tm_min) + ':';         // minute
    buffer_ += std::to_string(struct_time_now->tm_sec) + ' ';         // second
}

std::string &LogStream::get_buffer()
{
    return this->buffer_;
}

LogStream &LogStream::operator<<(std::string &&str)
{
    buffer_ += str;
    return *this;
}
LogStream &LogStream::operator<<(std::string &str)
{
    buffer_ += str;
    return *this;
}
// LogStream LogStream LogStream LogStream LogStream LogStream LogStream LogStream
