#include "client.h"

void delete_message(int socketfd,int id_chat, int id_message)
{
    cJSON* json = cJSON_CreateObject();

    cJSON_AddNumberToObject(json, "id_message", id_message);
    cJSON_AddNumberToObject(json, "id_chat", id_chat);

    req = DELETE_MESSAGE;
    send_request(socketfd, cJSON_Print(json), req);
}
