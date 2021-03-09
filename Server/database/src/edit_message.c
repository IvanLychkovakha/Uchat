#include "../inc/database.h"

static int callback(void *data, int argc, char **argv, char **cols)
{
    if (!(strcmp(argv[0], "1")))
    {
        cJSON_AddNumberToObject(data, cols[1], atoi(argv[1]));
        for (int i = 2; i < argc; i++)
        {
            cJSON_AddStringToObject(data, cols[i], argv[i]);
        }

        return 0;
    }
    return 1;
}


static void get_user_info(sqlite3 *db, cJSON *json_info)
{
    char *query = NULL;
    char *err = NULL;
    int rc = 0;

    cJSON *user = cJSON_CreateObject();
    asprintf(&query, "SELECT count(*), id_user, image, user_name, user_surname, phone FROM user_table WHERE id_user = '%i'",
             MX_VINT(json_info, "id_sender"));
    rc = sqlite3_exec(db, query, callback, user, &err);

    cJSON_AddItemToObject(json_info, "user", cJSON_Duplicate(user, 1));
    cJSON_Delete(user);
    free(query);
}
//message, id_message, id_sender, id_chat
cJSON *mx_edit_message(sqlite3 *db, cJSON *jsn)
{
    char *query = NULL;
    char *err = NULL;
    int rc = 0;
    cJSON *json_info = cJSON_CreateObject();
    asprintf(&query, "UPDATE message_table SET message = '%s' WHERE id_message = %i AND id_sender = %i;",
             MX_VSTR(jsn, "message"), MX_VINT(jsn, "id_message"), MX_VINT(jsn, "id_sender"));
    rc = sqlite3_exec(db, query, NULL, NULL, &err);
    if (mx_check(rc, err) != SQLITE_OK)
    {
        MX_SET_STATUS(jsn, 0);
        MX_SET_MESSAGE_STATUS(jsn, "Не удалось отредактировать сообщение!");
    }
    else
    {
        
        MX_SET_STATUS(jsn, 1);
        MX_SET_MESSAGE_STATUS(jsn, "Сообщение отредактировано!");
        cJSON *message = cJSON_CreateObject();
        cJSON_AddNumberToObject(message, "id_message", MX_VINT(jsn, "id_message"));
        cJSON_AddStringToObject(message, "message", MX_VSTR(jsn, "message"));
        cJSON_AddNumberToObject(message, "id_chat", MX_VINT(jsn, "id_chat"));
        cJSON_AddItemToObject(json_info, "message", cJSON_Duplicate(message,1));
        cJSON_AddNumberToObject(json_info, "id_chat", MX_VINT(jsn, "id_chat"));
        cJSON_AddNumberToObject(json_info, "id_sender", MX_VINT(jsn, "id_sender"));
        get_user_info(db, json_info);
        get_users_in_chat(db, json_info);
        cJSON_DeleteItemFromObject(json_info, "id_chat");
        cJSON_DeleteItemFromObject(json_info, "id_sender");
        cJSON_AddItemToObject(jsn, "json_info", cJSON_Duplicate(json_info,1));
        cJSON_Delete(message);
    }

    cJSON_DeleteItemFromObject(jsn, "id_sender");
    cJSON_DeleteItemFromObject(jsn, "id_chat");
    cJSON_DeleteItemFromObject(jsn, "id_message");
    cJSON_DeleteItemFromObject(jsn, "message");
    cJSON_Delete(json_info);
    free(query);
    return jsn;
}
