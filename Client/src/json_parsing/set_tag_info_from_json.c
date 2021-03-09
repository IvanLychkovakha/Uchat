#include "client.h"

void set_tag_info_from_json(const char *json, struct tag_info *tag)
{
    cJSON *item = cJSON_Parse(json);

    if(cJSON_HasObjectItem(item, "id_tag"))
        tag->id = cJSON_GetObjectItem(item, "id_tag")->valueint;

    if(cJSON_HasObjectItem(item, "tag_name"))
        tag->name = mx_strdup(cJSON_GetObjectItem(item, "tag_name")->valuestring);

    cJSON_Delete(item);
}
