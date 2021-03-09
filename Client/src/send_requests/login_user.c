#include "client.h"

void login_user(int socketfd, char* phone, char* password)
{
    cJSON* json = cJSON_CreateObject();

    cJSON_AddStringToObject(json, "phone", phone);
    cJSON_AddStringToObject(json, "user_password", password);

    req = LOGIN;
    send_request(socketfd, cJSON_Print(json), req);
}
