#include "../inc/database.h"
static int callback(void *data, int argc, char **argv, char **cols)
{
    if (!(strcmp(argv[0], "1")))
    {

        for (int i = 1; i < argc; i++)
        {
            cJSON_AddStringToObject(data, cols[i], argv[i]);
        }

        return 0;
    }
    return 1;
}
static int callback2(void *data, int argc, char **argv, char **cols)
{
    cJSON *chat = cJSON_CreateObject();
    cJSON_AddNumberToObject(chat, cols[0], atoi(argv[0]));
    cJSON_AddStringToObject(chat, cols[1], argv[1]);
    cJSON_AddStringToObject(chat, cols[3], argv[3]);
    cJSON_AddStringToObject(chat, cols[4], argv[4]);
    cJSON_AddStringToObject(chat, cols[5], argv[5]);

    cJSON_AddItemToObject(data,"chat",cJSON_Duplicate(chat, 1));
    cJSON_Delete(chat);
    return 0;
}
static void get_chat_info(sqlite3 *db, cJSON *json_info){
    char *query = NULL;
    char *err = NULL;
    int rc = 0;
    
        asprintf(&query, "SELECT chat.id_chat, group_chat.chat_g_name, chat_status, chat_image, message, send_at\
                    FROM (SELECT id_chat, message, send_at\
                    FROM message_table\
                    ORDER BY send_at DESC) AS T JOIN chat ON T.id_chat = chat.id_chat\
                    JOIN user_chat ON chat.id_chat = user_chat.id_chat\
                    JOIN group_chat ON user_chat.id_chat = group_chat. id_chat\
                    WHERE user_chat.id_chat = %i\
                    GROUP BY chat.id_chat;",\
                    MX_VINT(json_info, "id_chat"));
    rc = sqlite3_exec(db, query, callback2, json_info, &err);
    //cJSON_DeleteItemFromObject(json_info,"id_chat"); 
    free(query);


}
static void get_user_info(sqlite3 *db, cJSON *json_info)
{
    char *query = NULL;
    char *err = NULL;
    int rc = 0;

    cJSON *user = cJSON_CreateObject();
    asprintf(&query, "SELECT count(*), image, user_name, user_surname, phone FROM user_table WHERE id_user = '%i'",
             MX_VINT(json_info, "id_user"));
    rc = sqlite3_exec(db, query, callback, user, &err);
    cJSON_AddNumberToObject(user, "id_user", MX_VINT(json_info, "id_user"));
    cJSON_AddItemToObject(json_info, "user", cJSON_Duplicate(user, 1));
    cJSON_Delete(user);
    free(query);
}
//id_user, id_chat
cJSON *mx_add_member_by_id(sqlite3 *db, cJSON *jsn)
{
    char *query = NULL;
    char *err = NULL;
    int rc = 0;
    cJSON *json_info = cJSON_CreateObject();
    asprintf(&query, "INSERT INTO user_chat (id_user, id_chat, chat_status) VALUES(%i, %i, 1);",
                MX_VINT(jsn, "id_user"), MX_VINT(jsn, "id_chat"));
    rc = sqlite3_exec(db, query, NULL, NULL, &err);
    if (mx_check(rc, err) != SQLITE_OK)
    {
        MX_SET_STATUS(jsn, 0);
        MX_SET_MESSAGE_STATUS(jsn, "Не удалось добавить пользователя");
    }
    else
    {
        MX_SET_STATUS(jsn, 1);
        MX_SET_MESSAGE_STATUS(jsn, "Пользователь добавлен");
        cJSON_AddNumberToObject(json_info, "id_user", MX_VINT(jsn, "id_user"));
        get_user_info(db,json_info);
        cJSON_AddNumberToObject(json_info, "id_chat", MX_VINT(jsn, "id_chat"));
        get_chat_info(db,json_info);
        get_users_in_chat(db, json_info);
        cJSON_DeleteItemFromObject(json_info, "id_user");
        cJSON_AddItemToObject(jsn, "json_info", cJSON_Duplicate(json_info,1));
    }
    printf("RESULT\n%s\n", cJSON_Print(jsn));
    cJSON_DeleteItemFromObject(jsn, "id_user");
    cJSON_DeleteItemFromObject(jsn, "id_chat");
    free(query);
    return jsn;
}
