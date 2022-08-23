#include "Acceptor.h"
#include "Server.h"
#include "Eventloop.h"
#include "EventloopPool.h"

#define __thread_nun__ 2

int main(int argc, char **argv)
{
    const char *ip = argv[1];
    const char *port = argv[2];

    Eventloop Eventloop_; // io #pool
    EventloopPool Eventloop_pool_(&Eventloop_, __thread_nun__);

    Server server_(&Eventloop_pool_, ip, port);

    Eventloop_.loop();

    return 0;
}