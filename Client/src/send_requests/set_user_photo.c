#include "client.h"

void set_user_photo(int socketfd,int id_user, char* image)
{
    cJSON* json = cJSON_CreateObject();

    cJSON_AddNumberToObject(json, "id_user", id_user);
    cJSON_AddStringToObject(json, "image", image);

    req = SET_USER_PHOTO;
    send_request(socketfd, cJSON_Print(json), req);
}
