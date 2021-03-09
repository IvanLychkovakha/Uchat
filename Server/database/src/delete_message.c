#include "../inc/database.h"



//id_message, id_chat
cJSON *mx_delete_message(sqlite3 *db, cJSON *jsn)
{
    char *query = NULL;
    char *err = NULL;
    int rc = 0;
    cJSON *json_info = cJSON_CreateObject();
    asprintf(&query, "DELETE FROM message_table WHERE id_message = %i; \
             DELETE FROM message_status WHERE id_message = %i;",
             MX_VINT(jsn, "id_message"), MX_VINT(jsn, "id_message"));
    rc = sqlite3_exec(db, query, NULL, NULL, &err);
    if (mx_check(rc, err) != SQLITE_OK)
    {
        MX_SET_STATUS(jsn, 0);
        MX_SET_MESSAGE_STATUS(jsn, "Не удалось удалить  сообщение");
    }
    else
    {
        MX_SET_STATUS(jsn, 1);
        MX_SET_MESSAGE_STATUS(jsn, "Сообщение удалено");
        cJSON_AddNumberToObject(json_info, "id_message", MX_VINT(jsn, "id_message"));
        cJSON_AddNumberToObject(json_info, "id_chat", MX_VINT(jsn, "id_chat"));
        get_users_in_chat(db, json_info);
         cJSON_AddItemToObject(jsn, "json_info", cJSON_Duplicate(json_info,1));
    }

    cJSON_DeleteItemFromObject(jsn, "id_message");
    cJSON_DeleteItemFromObject(jsn, "id_chat");
    cJSON_Delete(json_info);
    free(query);
    return jsn;
}
