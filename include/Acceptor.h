#pragma once

class Acceptor
{
public:
    Acceptor(const char *ip, const char *port);
    int get_server_fd();

private:
    int server_fd_;
};