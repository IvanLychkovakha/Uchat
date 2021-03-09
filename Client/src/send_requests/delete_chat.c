#include "client.h"


void delete_chat(int socketfd, int id_chat, int chat_status)
{
    cJSON* json = cJSON_CreateObject();

    cJSON_AddNumberToObject(json, "id_chat", id_chat);
    cJSON_AddNumberToObject(json, "chat_status", chat_status);

    req = DELETE_CHAT;
    send_request(socketfd, cJSON_Print(json), req);
}
