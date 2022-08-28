#pragma once
#define LOG_TRACE                                                     \
    if (LogFrontAbstract::get_log_level() <= LogFrontAbstract::TRACE) \
    LogFrontAbstract(__FILE__, __LINE__, LogFrontAbstract::TRACE, __func__).stream()
#define LOG_DEBUG                                                     \
    if (LogFrontAbstract::get_log_level() <= LogFrontAbstract::DEBUG) \
    LogFrontAbstract(__FILE__, __LINE__, LogFrontAbstract::DEBUG, __func__).stream()
#define LOG_INFO                                                     \
    if (LogFrontAbstract::get_log_level() <= LogFrontAbstract::INFO) \
    LogFrontAbstract(__FILE__, __LINE__).stream()
#define LOG_WARN LogFrontAbstract(__FILE__, __LINE__, LogFrontAbstract::WARN).stream()
#define LOG_ERROR LogFrontAbstract(__FILE__, __LINE__, LogFrontAbstract::ERROR).stream()
#define LOG_FATAL LogFrontAbstract(__FILE__, __LINE__, LogFrontAbstract::FATAL).stream()
#define LOG_SYSERR LogFrontAbstract(__FILE__, __LINE__, false).stream()
#define LOG_SYSFATAL LogFrontAbstract(__FILE__, __LINE__, true).stream()

#include <string>
#include <iostream>

class LogFrontAbstract;
class LogFrontSpecial;
class LogStream;

extern LogFrontAbstract::LogLevel g_logLevel; // shengming

class LogFrontAbstract
{
public:
    enum LogLevel
    {
        TRACE = 0,
        DEBUG,
        INFO,
        WARN,
        ERROR,
        FATAL,
        NUM_LOG_LEVELS
    };

public:
    LogFrontAbstract(std::string file_name, int line_num, LogLevel level, std::string func_name);
    ~LogFrontAbstract();
    LogStream &stream();
    LogLevel get_log_level();
    void set_log_level(LogLevel level);

private:
    LogFrontSpecial log_special_;
};

class LogFrontSpecial
{
public:
private:
    LogStream stream_;
    LogFrontAbstract::LogLevel level_;
    int line_num_;
    std::string file_name_;
};

class LogStream
{
public:
    LogStream &operator<<(std::string str);

private:
    std::string buffer_;
    int buffer_size_max_;
};