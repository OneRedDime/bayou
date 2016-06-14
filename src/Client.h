#pragma once
#include <netdb.h>

class Client
{
    public:
        Client(const char *hostname, int port);
        int cOpen();
        int cWrite(const char *msg, int msg_size);
        int cRead(char **msg);
        int cClose();

    private:
        int sockfd, portno;
        hostent *server;
        sockaddr_in serv_addr;
};
