#include "client.h"


void set_chat_info_from_json(const char *json_chat, struct chat_info *chat)
{
    cJSON *item_chat = cJSON_Parse(json_chat);

    if(cJSON_HasObjectItem(item_chat, "id_chat"))
        chat->id = cJSON_GetObjectItem(item_chat, "id_chat")->valueint;

    if(cJSON_HasObjectItem(item_chat, "chat_name"))
        chat->name = mx_strdup(cJSON_GetObjectItem(item_chat, "chat_name")->valuestring);

    if(cJSON_HasObjectItem(item_chat, "chat_image"))
        chat->image = mx_strdup(cJSON_GetObjectItem(item_chat, "chat_image")->valuestring);

    if(cJSON_HasObjectItem(item_chat, "chat_status"))
        chat->type = cJSON_GetObjectItem(item_chat, "chat_status")->valueint;

    if(cJSON_HasObjectItem(item_chat, "message"))
        chat->last_message = mx_strdup(cJSON_GetObjectItem(item_chat, "message")->valuestring);

    if(cJSON_HasObjectItem(item_chat, "send_at"))
        chat->last_message_time = mx_strdup(cJSON_GetObjectItem(item_chat, "send_at")->valuestring);
    
    cJSON_Delete(item_chat);
}
