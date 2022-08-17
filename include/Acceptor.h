#pragma once

#include "Eventloop.h"
#include "Connector.h"

#include <memory>
#include <map>
#include <string>

class Acceptor
{
public:
    Acceptor(const char *ip, const char *port);
    void new_connection(Eventloop *loop);
    int get_fd();
    void set_nonnblocking(int fd);

private:
    int server_fd_;
    std::map<std::string, std::shared_ptr<Connector>> conn_map_;
};