#include"Channel.h"
class Eventloop;
class SocketFd;
class Channel;

class Server
{
public:
    Server(Eventloop*loop,int fd);
    ~Server();

private:
    Eventloop* loop_;
    
    Channel accept_channel_;

};