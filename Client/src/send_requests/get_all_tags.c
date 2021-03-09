#include "client.h"

void get_all_tags(int socketfd)
{
    cJSON* json = cJSON_CreateObject();
    
    req = GET_ALL_TAGS;
    send_request(socketfd, cJSON_Print(json), req);
}
