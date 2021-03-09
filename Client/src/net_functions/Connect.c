#include "uchat.h"

int Connect(int sockfg, const struct sockaddr *address, socklen_t address_len)
{
    int connectid = connect(sockfg, address, address_len);

    if(connectid == -1)
    {
        perror("Error: ");
        exit(EXIT_FAILURE);
    }
    connectid = 1;
    return connectid;
}

