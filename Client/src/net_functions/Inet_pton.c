#include "uchat.h"

int Inet_pton(int af, const char* restrict src, void* restrict dst)
{
    int inetid = inet_pton(af, src, dst);

    if(inetid <= 0)
    {
        printf("\nError: inet_pton error occured\n");
        exit(EXIT_FAILURE);
    }

    return inetid;
}
