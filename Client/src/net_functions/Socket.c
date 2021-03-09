#include "uchat.h"

int Socket(int domain, int type, int protocol)
{
    int socketfd = socket(domain, type, protocol);

    if(socketfd < 0)
    {
         printf("\n Error : Could not create socket \n");
         exit(EXIT_FAILURE);
    }

    return socketfd;
}
