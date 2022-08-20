
#include "Acceptor.h"
#include "Connector.h"

#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>

#include <iostream>
#include <time.h>

void Acceptor::set_nonnblocking(int fd)
{
    int flags = fcntl(fd, F_GETFL);
    flags |= O_NONBLOCK; // SETNONBLOCKING
    fcntl(fd, F_SETFL, flags);
}

void Acceptor::rm_conn_from_map(std::string str)
{
    // conn io
    std::shared_ptr<Connector> Conn=this->conn_map_[str];
    this->conn_map_.erase(str);

    // main io
    main_loop_->push_func(
        std::bind(&Connector::conn_destroy, Conn));

    // this->main_loop_();
    return;
}

Acceptor::Acceptor(const char *ip, const char *port, Eventloop *main_loop)
    : main_loop_(main_loop)
{
    struct sockaddr_in address_;
    memset(&address_, 0, sizeof(address_));
    address_.sin_family = AF_INET;
    inet_pton(AF_INET, ip, &address_.sin_addr);
    address_.sin_port = htons(atoi(port));

    this->server_fd_ = socket(PF_INET, SOCK_STREAM, 0);

    this->set_nonnblocking(server_fd_);

    int ret = bind(server_fd_, (struct sockaddr *)&address_, sizeof(address_));
    assert(ret != -1);

    ret = listen(server_fd_, 1000000);
    assert(ret != -1);
}

void Acceptor::new_connection(Eventloop *loop)
{
    struct sockaddr_in client;
    socklen_t client_addrlength = sizeof(client);

    int new_fd = accept(this->server_fd_,
                        (struct sockaddr *)(&client),
                        &client_addrlength);
    assert(new_fd > 0);

    this->set_nonnblocking(new_fd);

    std::string name = inet_ntoa(client.sin_addr);
    name = name + ' ' + std::to_string(client.sin_port);

    std::shared_ptr<Connector> s_new_conn = std::make_shared<Connector>(loop, client, new_fd, name);

    s_new_conn.get()->rm_call_back_to_acceptor = std::bind(&Acceptor::rm_conn_from_map, this, name); // callback

    this->conn_map_[name] = std::move(s_new_conn);
    
    std::cout << name << " --->   is the   new conn :in    " << __TIME__ << "..." << std::endl;
}

int Acceptor::get_fd()
{
    return this->server_fd_;
}
