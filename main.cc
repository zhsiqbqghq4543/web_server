#include "Acceptor.h"
#include "Server.h"
#include "Eventloop.h"


int main(int argc, char **argv)
{
    const char *ip = argv[1];
    const char *port = argv[2];

    Eventloop Eventloop_; // io #pool

    Server server_(&Eventloop_, ip, port);

    Eventloop_.loop();

    return 0;
}