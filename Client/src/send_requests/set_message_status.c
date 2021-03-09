#include "client.h"

void set_message_status(int socketfd, int id_message)
{
    cJSON* json = cJSON_CreateObject();

    cJSON_AddNumberToObject(json, "id_message", id_message);

    req = SET_MESSAGE_STATUS;
    send_request(socketfd, cJSON_Print(json), req);
}
