#include "client.h"


void search_by_tag_id(int socketfd, int id_tag, int id_user)
{
    cJSON* json = cJSON_CreateObject();
    cJSON_AddNumberToObject(json, "id_user", id_user);
    cJSON_AddNumberToObject(json, "id_tag", id_tag);
    req = SEARCH_USERS_BY_TAG_ID;
    
    send_request(socketfd, cJSON_Print(json), req);

    cJSON_Delete(json);
}
