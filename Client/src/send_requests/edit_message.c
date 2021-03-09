#include "client.h"

void edit_message(int socketfd, int id_message, char* message, int id_sender, int id_chat)
{
    cJSON* json = cJSON_CreateObject();
//message, id_message, id_sender, id_chat

    cJSON_AddNumberToObject(json, "id_sender", id_sender);
    cJSON_AddNumberToObject(json, "id_chat", id_chat);
    cJSON_AddNumberToObject(json, "id_message", id_message);
    cJSON_AddStringToObject(json, "message", message);

    req = EDIT_MESSAGE;
    send_request(socketfd, cJSON_Print(json), req);
}
