#include "client.h"


void set_respons_from_json(const char *json, struct respons *resp)
{
    cJSON *item = cJSON_Parse(json);

    resp->id = cJSON_GetObjectItem(item, "status")->valueint;
    resp->json_type = cJSON_GetObjectItem(item, "json_type")->valueint;
    resp->message = mx_strdup(cJSON_GetObjectItem(item, "message_status")->valuestring);
    if(resp->id)
    {
        cJSON* info = cJSON_GetObjectItem(item, "json_info");
        resp->json_info = mx_strdup(cJSON_Print(info));
    }
    
    cJSON_Delete(item);
}

