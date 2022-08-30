#include "Acceptor.h"
#include "Server.h"
#include "Eventloop.h"
#include "EventloopPool.h"

#define __thread_nun__ 2

#include "LogFront.h"
#include "LogBack.h"

LogBack Log;

void push_call_back(std::string &str)
{
    Log.append_to_buffer(str);
}

int main(int argc, char **argv)
{
    std::thread log_thread(&LogBack::thread_loop, &Log);

    log_global::func_ptr = push_call_back;

    const char *ip = argv[1];
    const char *port = argv[2];

    Eventloop Eventloop_; // io #pool
    EventloopPool Eventloop_pool_(&Eventloop_, __thread_nun__);

    Server server_(&Eventloop_pool_, ip, port);

    LOG_TRACE << "before Eventloop_.loop();";

    Eventloop_.loop();

    return 0;
}