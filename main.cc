#pragma once

#include "Acceptor.h"
#include "Server.h"
#include "Eventloop.h"

int main(int argc, char **argv)
{
    const char *ip = argv[1];
    const char *port = argv[2];

    Acceptor acceptor_(ip, port);

    Eventloop Eventloop_;//io

    Server server_(&Eventloop_,acceptor_.get_server_fd());

    return 0;
}