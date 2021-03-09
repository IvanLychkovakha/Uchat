#include "client.h"


void send_request(int socketfd, char* json, enum request_type req)
{
    cJSON *json_item = cJSON_CreateObject();
    json_item = cJSON_Parse(json);

    cJSON_AddNumberToObject(json_item, "json_type", req);
    
    send(socketfd, cJSON_Print(json_item), strlen(cJSON_Print(json_item)), MSG_DONTWAIT);
}
