#include "Client.h"
#include "communication.h"
#include <cstdio>
#include <unistd.h>
#include <string.h>

Client::Client(const char *host, int port)
{
    char hostname[256];
    strcpy(hostname, host);
    portno = port;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        // Errors
        printf("Error opening socket\n");
    }

    server = gethostbyname(hostname);
    if (!server)
    {
        // Errors
        printf("Could not get host %s\n", hostname);
    }
}

int Client::cOpen()
{
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *) &serv_addr.sin_addr.s_addr,
        server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
    {
        // Errors
        printf("Error connecting to port %d\n", portno);
        return -1;
    }

    return 0;
}

int Client::cWrite(const char *msg, int msg_size)
{
    return g_write(sockfd, msg, msg_size);
}

int Client::cRead(char **msg)
{
    return g_read(sockfd, msg);
}

int Client::cClose()
{
    if(close(sockfd) != 0)
    {
        printf("ERROR closing client socket %d\n", sockfd);
    }

    return 0;
}
