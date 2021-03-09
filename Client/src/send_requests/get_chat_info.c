#include "client.h"

void get_chat_info(int socketfd, int id_chat, int id_user)
{
    cJSON* json = cJSON_CreateObject();
    
    cJSON_AddNumberToObject(json, "id_chat", id_chat);
    cJSON_AddNumberToObject(json, "id_user", id_user);

    req = GET_CHAT_INFO;
    send_request(socketfd, cJSON_Print(json), req);
}

