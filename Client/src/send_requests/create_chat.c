#include "client.h"


void create_chat(int socketfd, char* chat_name, int id_creater, int type, char* image, int* participants, int parts_length)
{
    cJSON* json = cJSON_CreateObject();
    cJSON* parts_json_arr = cJSON_CreateIntArray(participants, parts_length);
    
    
    cJSON_AddStringToObject(json, "chat_name", chat_name);
    cJSON_AddNumberToObject(json, "id_creater", id_creater);
    cJSON_AddNumberToObject(json, "chat_status", type);
    cJSON_AddStringToObject(json, "chat_image", image);
    cJSON_AddItemToObject(json, "participants", parts_json_arr);

    req = CREATE_CHAT;
    send_request(socketfd, cJSON_Print(json), req);

    cJSON_Delete(json);
}
