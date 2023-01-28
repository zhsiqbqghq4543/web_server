#include "HttpRequest.h"

#include <iostream>
#include <fcntl.h>
#include <unistd.h>

HttpRequest::HttpRequest() : request_type_(Null),
                             version_(Unknown)
{
}

std::string HttpRequest::get_path()
{
    return this->path_;
}

std::string HttpRequest::get_type()
{
    if (this->request_type_ == Get)
        return "Get";
    else if (this->request_type_ == Post)
        return "POST";
    return "error";
}

std::string HttpRequest::get_version()
{
    if (this->version_ == Http10)
        return "1.0";
    else if (this->version_ == Http11)
        return "1.1";
    return "error";
}

void HttpRequest::set_type(std::string s)
{
    if (s == "GET")
    {
        request_type_ = Get;
    }
    else if (s == "POST")
    {
        request_type_ = Post;
    }
}
void HttpRequest::set_version(std::string s)
{
    if (s == "1.0")
    {
        version_ = Http10;
    }
    else if (s == "1.1")
    {
        version_ = Http11;
    }
}

bool HttpRequest::init_request_line(std::string &str)
{
    // std::cout << "\ninit_request_line\n";

    int ptr_r = str.find(' ');
    this->set_type(str.substr(0, ptr_r));

    // std::cout << str.substr(0, ptr_r) << std::endl;

    int ptr_l = str.find('/');
    ptr_r = str.find(' ', ptr_l) - 1;
    this->path_ = str.substr(ptr_l, ptr_r - ptr_l + 1);
    //std::cout<<this->path_<<std::endl;
    // std::cout << str.substr(ptr_l, ptr_r - ptr_l + 1) << std::endl;

    this->set_version(str.substr(str.size() - 5, 3));

    // std::cout << str.substr(str.size()-5, 3) << std::endl;

    if (this->version_ != Unknown && this->request_type_ != Null)
        return true;
    return false;
}

bool HttpRequest::push_header_line(std::string &str)
{
    this->vec.push_back(str);
    return true;
}

bool HttpRequest::push_body_line(std::string &str)
{
    // TODO
    return true;
}

std::string HttpRequest::get_send_data()
{
    //std::cout<<this->get_path()<<std::endl;

    if (this->get_path() == "/")
    {
        this->path_ = "/login.html";
    }

    std::string file_path = "../data" + this->get_path();
    int send_html_fd = open(file_path.c_str(), O_RDONLY);
    if (send_html_fd == -1||this->path_ == "/404")
    {
        close(send_html_fd);
        std::string send_data;
        send_data = "HTTP/" + this->get_version() + " 404 Not Found\r\n";
        send_data += "Content-Length: " + std::to_string(0) + "\r\n\r\n";
        return send_data;
    }
    int size = lseek(send_html_fd, 0, SEEK_END);
    lseek(send_html_fd, 0, SEEK_SET);
    std::string html_data(size + 5, ' ');
    read(send_html_fd, &*html_data.begin(), html_data.size());
    std::string send_data;
    send_data.reserve(4000);
    send_data = "HTTP/" + this->get_version() + " 200 OK\r\n";
    send_data += "Server: Ubuntu\r\n";
    send_data += "Content-Type: text/html; charset=UTF-8\r\n";
    send_data += "Content-Length: " + std::to_string(size) + "\r\n\r\n";
    send_data += html_data;

    close(send_html_fd);
    return std::move(send_data);
}
