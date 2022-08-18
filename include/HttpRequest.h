#pragma once

#include <string>

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
        http11
    };
    HttpRequest();

private:
    RequestType request_type_;
    Version version_;
    std::string path_;
};