#include "client.h"


void add_member_by_id(int socketfd, int id_user, int id_chat)
{
    cJSON* json = cJSON_CreateObject();

    cJSON_AddNumberToObject(json, "id_user", id_user);
    cJSON_AddNumberToObject(json, "id_chat", id_chat);


    req = ADD_MEMBER_BY_ID;

    send_request(socketfd, cJSON_Print(json), req);
}

