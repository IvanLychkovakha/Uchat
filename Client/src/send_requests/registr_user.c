#include "client.h"

void registr_user(struct client_info* client, int socketfd, char *user_name, char* user_surname, char* user_password, char* phone, char* image, int* tagsid, int tags_length)
{   
    client->current_user->name = mx_strdup(user_name);
    client->current_user->lastname = mx_strdup(user_surname);
    client->current_user->phone_number = mx_strdup(phone);
    client->current_user->image = mx_strdup(image);


    cJSON* json = cJSON_CreateObject();
    cJSON* tags_id = cJSON_CreateIntArray(tagsid, tags_length);

    cJSON_AddStringToObject(json, "user_name", user_name);
    cJSON_AddStringToObject(json, "user_surname", user_surname);
    cJSON_AddStringToObject(json, "user_password", user_password);
    cJSON_AddStringToObject(json, "phone", phone);
    cJSON_AddStringToObject(json, "image", image);
    cJSON_AddItemToObject(json, "tags", tags_id);

    req = REGISTR;
    send_request(socketfd, cJSON_Print(json), req);

    cJSON_Delete(json);
}
