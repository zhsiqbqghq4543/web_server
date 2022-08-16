#pragma once

#include <string.h>
#include <assert.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>

#include "Acceptor.h"

Acceptor::Acceptor(const char *ip, const char *port)
{
    struct sockaddr_in address_;
    memset(&address_, 0, sizeof(address_));
    address_.sin_family = AF_INET;
    inet_pton(AF_INET, ip, &address_.sin_addr);
    address_.sin_port = atoi(port);

    server_fd_ = socket(PF_INET, SOCK_STREAM, 0);

    int ret = bind(server_fd_, (struct sockaddr *)&address_, sizeof(address_));
    assert(ret != -1);

    ret = listen(server_fd_, 2000000);
    assert(ret != -1);
}

int Acceptor::get_server_fd()
{
    return server_fd_;
}