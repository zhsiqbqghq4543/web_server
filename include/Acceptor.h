#pragma once

#include "Eventloop.h"
#include "Connector.h"

#include <memory>
#include <map>
#include <string>
#include<functional>

class Acceptor
{
public:
    Acceptor(const char *ip, const char *port, Eventloop *main_loop);
    void new_connection(Eventloop *loop);
    int get_fd();
    void set_nonnblocking(int fd);
    void rm_conn_from_map(std::shared_ptr<Connector> Conn);
private:
    Eventloop *main_loop_;
    int server_fd_;
    std::map<std::string, std::shared_ptr<Connector>> conn_map_;
};