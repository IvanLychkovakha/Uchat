#include "client.h"


void set_message_info_from_json(const char *json, struct message_info *message)
{
    cJSON *item_message = cJSON_Parse(json);

    if(cJSON_HasObjectItem(item_message, "id_message"))
        message->id = cJSON_GetObjectItem(item_message, "id_message")->valueint;

    if(cJSON_HasObjectItem(item_message, "message"))
        message->text = mx_strdup(cJSON_GetObjectItem(item_message, "message")->valuestring);

    if(cJSON_HasObjectItem(item_message, "send_at"))
        message->date = mx_strdup(cJSON_GetObjectItem(item_message, "send_at")->valuestring);

    if(cJSON_HasObjectItem(item_message, "is_read"))
        message->is_read = cJSON_GetObjectItem(item_message, "is_read")->valueint;

    cJSON_Delete(item_message);
}
