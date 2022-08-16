#pragma once

#include"Server.h"

Server::Server(Eventloop*loop,int fd):accept_channel_(loop,fd)
{
    
}