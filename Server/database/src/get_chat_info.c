#include "../inc/database.h"

static int callback(void *data, int argc, char **argv, char **cols)
{
    /*for(int i = 0; i < argc; i++){
        printf("%s:%s\n",cols[i],argv[i]);
    }*/
    cJSON *json_row = cJSON_CreateObject();

    cJSON_AddNumberToObject(json_row, cols[0], atoi(argv[0]));
    cJSON_AddStringToObject(json_row, cols[1], argv[1]);
    cJSON_AddStringToObject(json_row, cols[2], argv[2]); // +11
    cJSON_AddNumberToObject(json_row, cols[3], atoi(argv[3]));
    cJSON_AddNumberToObject(json_row, cols[4], atoi(argv[4]));
    cJSON_AddItemToArray(data, cJSON_Duplicate(json_row, 1));
    cJSON_Delete(json_row);
    return 0;
}
static int callback2(void *data, int argc, char **argv, char **cols)
{
    /*for(int i = 0; i < argc; i++){
        printf("%s:%s\n",cols[i],argv[i]);
    }*/
    cJSON *chat = cJSON_CreateObject();
    cJSON_AddNumberToObject(chat, cols[0], atoi(argv[0]));
    cJSON_AddStringToObject(chat, "chat_name", argv[1]);
    cJSON_AddNumberToObject(chat, cols[2], atoi(argv[2]));
    cJSON_AddStringToObject(chat, cols[3], argv[3]);
    cJSON_AddItemToObject(data, "chat", cJSON_Duplicate(chat, 1));
    cJSON_Delete(chat);
    return 0;
}
static int callback3(void *data, int argc, char **argv, char **cols)
{
    /*for(int i = 0; i < argc; i++){
        printf("%s:%s\n",cols[i],argv[i]);
    }*/
    cJSON *json_row = cJSON_CreateObject();
    cJSON_AddNumberToObject(json_row, cols[0], atoi(argv[0]));
    cJSON_AddStringToObject(json_row, cols[1], argv[1]);
    cJSON_AddStringToObject(json_row, cols[2], argv[2]);
    cJSON_AddStringToObject(json_row, cols[3], argv[3]);
    cJSON_AddItemToArray(data, cJSON_Duplicate(json_row, 1));
    cJSON_Delete(json_row);
    return 0;
}
static int callback5(void *data, int argc, char **argv, char **cols)
{
    cJSON *json_row = cJSON_CreateObject();
    cJSON_AddNumberToObject(json_row, cols[0],atoi(argv[0]));
    cJSON_AddStringToObject(json_row, cols[1],argv[1]);
    cJSON_AddItemToArray(data,cJSON_Duplicate(json_row, 1));
    cJSON_Delete(json_row); 

    return 0;
}
static void get_tags(sqlite3 *db, cJSON *json_info, int id_user)
{
    char *query = NULL;
    char *err = NULL;
    int rc = 0;
   cJSON *tags = cJSON_CreateArray();

    asprintf(&query, "SELECT * FROM tags\
                    JOIN user_tag ON tags.id_tag = user_tag.id_tag\
                    JOIN user_table ON user_tag.id_user = user_table.id_user\
                    WHERE user_tag.id_user = %d;",
                    id_user);
    rc = sqlite3_exec(db, query, callback5, tags, &err);
    cJSON_AddItemToObject(json_info, "tags", cJSON_Duplicate(tags, 1));
    cJSON_Delete(tags);
    //free(phone);
    free(query);
}
static void get_users_info_in_chat(sqlite3 *db, cJSON *json_info)
{
    char *query = NULL;
    char *err = NULL;
    int rc = 0;
    cJSON *users = cJSON_CreateArray();
    // printf("id:%i\n",MX_VINT(jsn, "id_chat") );

    asprintf(&query, "SELECT user_chat.id_user, user_name, user_surname, image FROM user_chat \
                    INNER JOIN user_table ON user_chat.id_user = user_table.id_user \
                    WHERE id_chat = %i;",
             MX_VINT(json_info, "id_chat"));
    rc = sqlite3_exec(db, query, callback3, users, &err);

    for (int i = 0; i < cJSON_GetArraySize(users); i++)
    {
        cJSON *row = cJSON_GetArrayItem(users, i);

        get_tags(db, row, MX_VINT(row, "id_user"));
    }
    cJSON_AddItemToObject(json_info, "users", cJSON_Duplicate(users, 1));

    cJSON_Delete(users);
    free(query);
}
//id_chat,
static void get_messages(sqlite3 *db, cJSON *jsn, cJSON *json_info)
{
    char *query = NULL;
    char *err = NULL;
    int rc = 0;

    cJSON *messages = cJSON_CreateArray();

    //printf("id:%i\n",MX_VINT(jsn, "id_chat") );
    asprintf(&query, "SELECT message_table.id_message, message, send_at, id_sender, is_read \
                    FROM message_table\
                    JOIN message_status ON message_table.id_message = message_status.id_message \
                    JOIN user_table ON message_table.id_sender = user_table.id_user \
                    WHERE id_chat = %i AND message_status.id_user = %i\
                    ORDER BY send_at;",
             MX_VINT(jsn, "id_chat"), MX_VINT(jsn, "id_user"));
    rc = sqlite3_exec(db, query, callback, messages, &err);

    cJSON_AddItemToObject(json_info, "messages", cJSON_Duplicate(messages, 1));

    cJSON_Delete(messages);
    free(query);
}

//id_chat, id_user
cJSON *mx_get_chat_info(sqlite3 *db, cJSON *jsn)
{

    char *query = NULL;
    char *err = NULL;
    int rc = 0;
    cJSON *json_info = cJSON_CreateObject();

    int chat_id = MX_VINT(jsn, "id_chat");
    int user_id = MX_VINT(jsn, "id_user");

    asprintf(&query, "SELECT chat.id_chat, private_chat.chat_p_name, chat_status, chat_image  \
                    FROM (SELECT id_chat, message, send_at \
                    FROM message_table \
                    ORDER BY send_at DESC) AS T JOIN chat ON T.id_chat = chat.id_chat\
                    JOIN user_chat ON chat.id_chat = user_chat.id_chat\
                    JOIN private_chat ON user_chat.id_chat = private_chat. id_chat\
                    WHERE user_chat.id_chat = %i\
                    GROUP BY chat.id_chat\
                    UNION\
                    SELECT chat.id_chat, group_chat.chat_g_name, chat_status, chat_image \
                    FROM (SELECT id_chat, message, send_at\
                    FROM message_table\
                    ORDER BY send_at DESC) AS T JOIN chat ON T.id_chat = chat.id_chat\
                    JOIN user_chat ON chat.id_chat = user_chat.id_chat\
                    JOIN group_chat ON user_chat.id_chat = group_chat. id_chat\
                    WHERE user_chat.id_chat = %i\
                    GROUP BY chat.id_chat",
             chat_id, chat_id);
    rc = sqlite3_exec(db, query, callback2, json_info, &err);

    if (mx_check(rc, err) != SQLITE_OK)
    {
        MX_SET_STATUS(jsn, 0);
        MX_SET_MESSAGE_STATUS(jsn, "Не удалось получить информацию о чате");
    }
    else
    {

        MX_SET_STATUS(jsn, 1);
        MX_SET_MESSAGE_STATUS(jsn, "Получена информация о чате");

        cJSON_AddNumberToObject(json_info, "id_chat", chat_id);
        get_users_info_in_chat(db, json_info);
        cJSON_AddNumberToObject(json_info, "id_user", user_id);
        get_messages(db, jsn, json_info);

        //cJSON_DeleteItemFromObject(json_info, "id_chat");
        cJSON_DeleteItemFromObject(json_info, "id_user");
        cJSON_AddItemToObject(jsn, "json_info", cJSON_Duplicate(json_info, 1));
    }

    cJSON_DeleteItemFromObject(jsn, "id_chat");
    cJSON_DeleteItemFromObject(jsn, "id_user");
    cJSON_Delete(json_info);

    free(query);
    return jsn;
}
