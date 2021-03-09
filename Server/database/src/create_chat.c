#include "../inc/database.h"

static void insert_into_userchat(sqlite3 *db, cJSON *jsn, int last_id)
{
    cJSON *participants = cJSON_GetObjectItem(jsn, "participants");
    char *query = NULL;
    char *err = NULL;
    int rc = 0;

    int creator_id = MX_VINT(jsn, "id_creater");
    int chat_type = MX_VINT(jsn, "chat_status");

    asprintf(&query, "INSERT INTO user_chat (id_user, id_chat, chat_status)\
                    VALUES (%i, %i, %i);",
             creator_id, last_id, chat_type);
    rc = sqlite3_exec(db, query, NULL, NULL, &err);
    free(query);
    for (int i = 0; i < cJSON_GetArraySize(participants); i++)
    {
        int id_user = cJSON_GetArrayItem(participants, i)->valueint;
        printf("id_user:%d\n", id_user);
        asprintf(&query, "INSERT INTO user_chat (id_user, id_chat, chat_status)\
                    VALUES (%i, %i, %i);",
                   id_user, last_id, chat_type);
        rc = sqlite3_exec(db, query, NULL, NULL, &err);
        free(query);
    }
}
static void create_group_or_private_chat(sqlite3 *db, cJSON *jsn, int last_id)
{
    char *query = NULL;
    char *err = NULL;
    int rc = 0;

    char *chat_name = MX_VSTR(jsn, "chat_name");
    char *chat_image = MX_VSTR(jsn, "chat_image");

    if (MX_VINT(jsn, "chat_status") == 1)
    {
        asprintf(&query, "INSERT INTO group_chat (id_chat, chat_g_name, chat_image)\
                    VALUES (%d, '%s', '%s');",
                 last_id, chat_name, chat_image);
    }
    else if (MX_VINT(jsn, "chat_status") == 0)
    {
        cJSON *participants = cJSON_GetObjectItem(jsn, "participants");
        int creator_id = MX_VINT(jsn, "id_creater");
        int participant_id = cJSON_GetArrayItem(participants, 0)->valueint;
        printf("part_id:%d\n", participant_id);
        int chat_type = MX_VINT(jsn, "chat_status");

        asprintf(&query, "INSERT INTO private_chat(id_user, id_chat, chat_p_name, chat_image)\
                    VALUES (%d, %d, '%s', '%s'),(%d, %d, '%s', '%s');",
                 creator_id, last_id, chat_name, chat_image,
                 participant_id, last_id, chat_name, chat_image);
    }
    rc = sqlite3_exec(db, query, NULL, NULL, &err);

    //printf("hello\n");
    /*free(chat_image);
    free(chat_name);*/
    free(query);
}
// id_creater, chat_name,id_second_user, chat_status, chat_image
cJSON *mx_create_chat(sqlite3 *db, cJSON *jsn)
{
    char *query = NULL;
    char *err = NULL;
    int rc = 0;
    printf("still work\n");
    int creator_id = MX_VINT(jsn, "id_creater");
    int chat_type = MX_VINT(jsn, "chat_status");
    char *chat_name = MX_VSTR(jsn, "chat_name");
    char *chat_image = MX_VSTR(jsn, "chat_image");

    cJSON *json_info = cJSON_CreateObject();
    cJSON *chat = cJSON_CreateObject();
    asprintf(&query, "INSERT INTO chat(id_creater) VALUES (%i);", creator_id);
    rc = sqlite3_exec(db, query, NULL, NULL, &err);
    if (mx_check(rc, err) != SQLITE_OK)
    {
        MX_SET_STATUS(jsn, 0);
        MX_SET_MESSAGE_STATUS(jsn, "Не удалось создать чат!");
        free(query);
    }
    else
    {
        printf("still work1\n");
        MX_SET_STATUS(jsn, 1);
        MX_SET_MESSAGE_STATUS(jsn, "Чат создан!");
        int last_id = sqlite3_last_insert_rowid(db);
        insert_into_userchat(db, jsn, last_id);
        printf("still work2\n");
        create_group_or_private_chat(db, jsn, last_id);
        printf("still work3\n");
        cJSON_AddNumberToObject(json_info, "id_chat", last_id);
        cJSON_AddNumberToObject(chat, "id_chat", last_id);
        cJSON_AddNumberToObject(chat, "chat_status", chat_type);
        cJSON_AddStringToObject(chat, "chat_name", chat_name);
        cJSON_AddStringToObject(chat, "chat_image", chat_image);
        cJSON_AddItemToObject(json_info, "chat", cJSON_Duplicate(chat, 1));
        printf("still work4\n");
        get_users_in_chat(db, json_info);
        printf("still work5\n");
        cJSON_AddItemToObject(jsn, "json_info", cJSON_Duplicate(json_info, 1));
        cJSON_DeleteItemFromObject(json_info, "id_chat");
    }
     cJSON_DeleteItemFromObject(jsn, "participants");
    cJSON_DeleteItemFromObject(jsn, "id_creater");
    cJSON_DeleteItemFromObject(jsn, "chat_name");
    cJSON_DeleteItemFromObject(jsn, "chat_status");
    cJSON_DeleteItemFromObject(jsn, "chat_image");
    cJSON_Delete(json_info);
    /*free(chat_name);
    free(chat_image);*/
    free(query);
    return jsn;
}
