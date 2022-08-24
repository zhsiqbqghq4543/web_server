#pragma once

#include <string>
#include <vector>
class HttpRequest
{
public:
    enum RequestType
    {
        Null,
        Get,
        Post
    };
    enum Version
    {
        Unknown,
        Http10,
        Http11
    };
    HttpRequest();
    bool init_request_line(std::string &str);
    bool push_header_line(std::string &str);
    bool push_body_line(std::string &str);
    void set_type(std::string s);
    void set_version(std::string s);
    std::string get_type();
    std::string get_version();
    std::string get_path();
    std::string get_send_data();

private:
    RequestType request_type_;
    Version version_;

    std::string path_;
    std::vector<std::string> vec;
};