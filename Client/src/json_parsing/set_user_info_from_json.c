#include "client.h"

void set_user_info_from_json(const char *json, struct user_info *user)
{
    cJSON *item = cJSON_Parse(json);
    if(cJSON_HasObjectItem(item, "id_user"))
        user->id = cJSON_GetObjectItem(item, "id_user")->valueint;

    if(cJSON_HasObjectItem(item, "phone"))
        user->phone_number = mx_strdup(cJSON_GetObjectItem(item, "phone")->valuestring);

    if(cJSON_HasObjectItem(item, "user_name"))
        user->name = mx_strdup(cJSON_GetObjectItem(item, "user_name")->valuestring);

    if(cJSON_HasObjectItem(item, "user_surname"))
        user->lastname = mx_strdup(cJSON_GetObjectItem(item, "user_surname")->valuestring);

    if(cJSON_HasObjectItem(item, "image"))
        user->image = mx_strdup(cJSON_GetObjectItem(item, "image")->valuestring);


    if(cJSON_HasObjectItem(item, "tags"))
    {
        cJSON* tags = cJSON_GetObjectItem(item, "tags");
        int tags_size = cJSON_GetArraySize(tags);

        user->tags = (struct tag_info**) realloc(user->tags, sizeof(struct tag_info*) * (tags_size + 1));
        user->tags[tags_size] = NULL;

        for (int i = 0; i < tags_size; i++)
        {
            user->tags[i] = dup_tag_info();
            set_tag_info_from_json(cJSON_Print(cJSON_GetArrayItem(tags, i)), user->tags[i]);
        }
    }
    cJSON_Delete(item);
}
