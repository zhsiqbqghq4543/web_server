#pragma once

#define LOG_TRACE                                                          \
    if (LogFrontAbstract::get_log_level() <= (int)LogFrontAbstract::TRACE) \
    LogFrontAbstract(__FILE__, __LINE__, LogFrontAbstract::TRACE, __func__).stream()
#define LOG_DEBUG                                                          \
    if (LogFrontAbstract::get_log_level() <= (int)LogFrontAbstract::DEBUG) \
    LogFrontAbstract(__FILE__, __LINE__, LogFrontAbstract::DEBUG, __func__).stream()
#define LOG_INFO                                                          \
    if (LogFrontAbstract::get_log_level() <= (int)LogFrontAbstract::INFO) \
    LogFrontAbstract(__FILE__, __LINE__).stream()
#define LOG_WARN LogFrontAbstract(__FILE__, __LINE__, LogFrontAbstract::WARN).stream()
#define LOG_ERROR LogFrontAbstract(__FILE__, __LINE__, LogFrontAbstract::ERROR).stream()
#define LOG_FATAL LogFrontAbstract(__FILE__, __LINE__, LogFrontAbstract::FATAL).stream()
#define LOG_SYSERR LogFrontAbstract(__FILE__, __LINE__, false).stream()
#define LOG_SYSFATAL LogFrontAbstract(__FILE__, __LINE__, true).stream()

#include <string>
#include <iostream>
#include <memory>
#include <functional>

class LogFrontAbstract;
class LogFrontSpecial;
class LogStream;

namespace log_global
{
    extern int g_logLevel;
    extern void (*func_ptr)(std::string &str);

}

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
    LogFrontAbstract(std::string file_name,
                     int line_num,
                     LogLevel level,
                     std::string func_name);
    ~LogFrontAbstract();
    LogStream &stream();
    static int get_log_level()
    {

        return log_global::g_logLevel;
    }

    void set_log_level(LogLevel level);
    std::string get_level_str(LogLevel level);

private:
    std::unique_ptr<LogFrontSpecial> log_special_;
};

class LogFrontSpecial
{
public:
    LogFrontSpecial(std::string &file_name,
                    int line_num,
                    std::string level,
                    int level_num);
    void finish();
    std::unique_ptr<LogStream> stream_;
    int get_level_num();

private:
    int level_num_;
    std::string level_;
    int line_num_;
    std::string file_name_;
};

class LogStream
{
public:
    LogStream();
    LogStream &operator<<(std::string &&str);
    LogStream &operator<<(std::string &str);
    std::string &get_buffer();
private:
    std::string buffer_;
    int buffer_size_max_;
};
