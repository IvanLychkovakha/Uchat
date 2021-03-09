#include "../inc/database.h"

static int callback(void *data, int argc, char **argv, char **cols)
{
    cJSON_AddNumberToObject(data, cols[0], atoi(argv[0]));
    return 0;
}
static int callback2(void *data, int argc, char **argv, char **cols)
{
    
    return strcmp(argv[0], "1") != 0;
}
static void get_id_creater(sqlite3 *db, cJSON *json_info)
{
    char *query = NULL;
    char *err = NULL;
    int rc = 0;
    asprintf(&query, "SELECT id_creater FROM chat WHERE id_chat = %i",
             MX_VINT(json_info, "id_chat"));
    rc = sqlite3_exec(db, query, callback, json_info, &err);
}

static void delete_group_or_private_chat(sqlite3 *db, cJSON *jsn)
{
    char *query = NULL;
    char *err = NULL;
    int rc = 0;
    if (MX_VINT(jsn, "chat_status") == 1)
    {
        asprintf(&query, "DELETE FROM group_chat WHERE id_chat = %i;",
                 MX_VINT(jsn, "id_chat"));
    }
    else
    {
        asprintf(&query, "DELETE FROM private_chat WHERE id_chat = %i;",
                 MX_VINT(jsn, "id_chat"));
    }
    rc = sqlite3_exec(db, query, NULL, NULL, &err);
    free(query);
}
//id_chat, chat_status
cJSON *mx_delete_chat(sqlite3 *db, cJSON *jsn)
{
    printf("%s\n", cJSON_Print(jsn));
    char *query = NULL;
    char *err = NULL;
    int rc = 0;
    cJSON *json_info = cJSON_CreateObject();

    int chat_id = MX_VINT(jsn, "id_chat");
    int chat_type = MX_VINT(jsn, "chat_status");
    asprintf(&query, "SELECT count(*) FROM chat WHERE id_chat = %d;", chat_id);
    rc = sqlite3_exec(db, query, callback2, NULL, &err);
    if (mx_check(rc, err) != SQLITE_OK)
    {
        MX_SET_STATUS(jsn, 0);
        MX_SET_MESSAGE_STATUS(jsn, "Такого чата не существует!");
        //logging(log, INFO, NULL, "Registration of user {name: %s, surname: %s, phone: %s, password: %s} was failed.", user_name, user_surname, phone, user_password);
    }
    else
    {
        free(query);
        cJSON_AddNumberToObject(json_info, "id_chat", chat_id);
        get_id_creater(db, json_info);
        get_users_in_chat(db, json_info);
        asprintf(&query, "DELETE FROM chat WHERE id_chat = %i; \
            DELETE FROM user_chat WHERE id_chat = %i;",
                 chat_id, chat_id);
        rc = sqlite3_exec(db, query, NULL, NULL, &err);
        if (mx_check(rc, err) != SQLITE_OK)
        {
            MX_SET_STATUS(jsn, 0);
            MX_SET_MESSAGE_STATUS(jsn, "Не удалось удалить  чат");
            //logging(log, INFO, NULL, "Chat with {chat_id: %d, chat_type: %d} wasn't deleted.", chat_id, chat_type);
        }
        else
        {
            MX_SET_STATUS(jsn, 1);
            MX_SET_MESSAGE_STATUS(jsn, "Чат успешно удален");
            //logging(log, INFO, NULL, "Chat with {chat_id: %d, chat_type: %d} successfully deleted.", chat_id, chat_type);
            delete_group_or_private_chat(db, jsn);
            cJSON_AddItemToObject(jsn, "json_info", cJSON_Duplicate(json_info, 1));
        }
    }
    cJSON_DeleteItemFromObject(jsn, "id_chat");
    cJSON_DeleteItemFromObject(jsn, "chat_status");
    cJSON_Delete(json_info);
    free(query);
    return jsn;
}
