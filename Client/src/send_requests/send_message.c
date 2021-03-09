#include "client.h"

void send_message(int socketfd, int id_user, int id_chat, char* message)
{
    cJSON* json = cJSON_CreateObject();

    cJSON_AddNumberToObject(json, "id_sender", id_user);
    cJSON_AddNumberToObject(json, "id_chat", id_chat);
    cJSON_AddStringToObject(json, "message", message);

    req = SEND_MESSAGE;
    send_request(socketfd, cJSON_Print(json), req);
}
