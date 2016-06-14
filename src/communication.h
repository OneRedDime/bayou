#pragma once

int g_read(int sockfd, char **msg);
int g_write(int sockfd, const char *msg, int msg_size);
