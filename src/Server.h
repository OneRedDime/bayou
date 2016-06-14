#pragma once
#include <vector>
#include <mutex>
#include <netdb.h>

class Server
{
    public:
        Server(int port, int num_conns);
        ~Server();
        int sListen();
        int sAccept();
        int sRead(int cli_num, char **buffer);
        int sWrite(int cli_num, const char *msg, int msg_size);
        int sClose(int cli_num);
        int sCloseAll();

    private:
        int num_conns;
        int sockfd, portno;
        sockaddr_in serv_addr, cli_addr;
        socklen_t clilen;

        std::vector<int> newsockfds;
        std::mutex mtx;
};
