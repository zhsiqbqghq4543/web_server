#include "Channel.h"

class Server
{
public:
    class new_conn_cb; // call_back

public:
    Server(Eventloop *loop, int fd);
    ~Server();

private:
    Eventloop *loop_;
    Channel accept_channel_;
};