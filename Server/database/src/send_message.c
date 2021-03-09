#include "../inc/database.h"

static void set_message_status(sqlite3 *db, int id_chat, int count)
{
    char *query = NULL;
    char *err = NULL;
    int rc = 0;
    asprintf(&query, "INSERT INTO message_status \
            VALUES((SELECT id_message FROM message_table WHERE id_chat = %d ORDER BY send_at DESC LIMIT 1), \
            (SELECT id_sender FROM message_table WHERE id_chat = %d AND id_message = \
            (SELECT id_message FROM message_table WHERE id_chat = %d ORDER BY send_at DESC LIMIT 1)), 1);",
             id_chat, id_chat, id_chat);
    rc = sqlite3_exec(db, query, NULL, NULL, &err);
    free(query);
    asprintf(&query, "INSERT INTO message_status \
            VALUES ((SELECT id_message FROM message_table \
            WHERE id_chat = %d ORDER BY send_at DESC LIMIT 1), \
            (SELECT id_user \
            FROM user_chat AS us JOIN chat AS c ON us.id_chat = c.id_chat \
            JOIN message_table AS mt ON c.id_chat = mt.id_chat \
            WHERE mt.id_chat = %d AND id_user != id_sender AND id_message = \
            (SELECT id_message FROM message_table \
            WHERE id_chat = %d ORDER BY send_at DESC LIMIT 1) \
            AND id_user NOT IN (SELECT id_user FROM message_status WHERE id_message = \
            (SELECT id_message FROM message_table \
            WHERE id_chat = %d ORDER BY send_at DESC LIMIT 1))\
            LIMIT 1), 0);",
             id_chat, id_chat, id_chat, id_chat);
    for (int i = 0; i < count; i++)
    {
        rc = sqlite3_exec(db, query, NULL, NULL, &err);
    }
    free(query);
}
static int callback(void *data, int argc, char **argv, char **cols)
{
    return strcmp(argv[0], "1") != 0;
}
static int callback2(void *data, int argc, char **argv, char **cols)
{
    cJSON *json_row = cJSON_CreateObject();

    cJSON_AddNumberToObject(json_row, cols[0], atoi(argv[0]));
    cJSON_AddItemToArray(data, cJSON_Duplicate(json_row, 1));
    cJSON_Delete(json_row);

    return 0;
}
static int callback3(void *data, int argc, char **argv, char **cols)
{
    cJSON_AddStringToObject(data, cols[0], argv[0]);
    return 0;
}
static int callback4(void *data, int argc, char **argv, char **cols)
{
    cJSON_AddStringToObject(data, cols[0], argv[0]);
    cJSON_AddStringToObject(data, cols[1], argv[1]);
    cJSON_AddStringToObject(data, cols[2], argv[2]);
    cJSON_AddStringToObject(data, cols[3], argv[3]);
    return 0;
}
static void get_message_time(sqlite3 *db, cJSON *message)
{
    char *query = NULL;
    char *err = NULL;
    int rc = 0;
    asprintf(&query, "SELECT send_at FROM message_table WHERE id_message = %i",
             MX_VINT(message, "id_message"));
    rc = sqlite3_exec(db, query, callback3, message, &err);
}
static void get_sender_info(sqlite3 *db, cJSON *user)
{
    char *query = NULL;
    char *err = NULL;
    int rc = 0;
    asprintf(&query, "SELECT user_name, user_surname, image, phone FROM user_table WHERE id_user = %i",
             MX_VINT(user, "id_user"));
    rc = sqlite3_exec(db, query, callback4, user, &err);
}
// id_chat, id_sender, message
cJSON *mx_send_message(sqlite3 *db, cJSON *jsn)
{
    char *query = NULL;
    char *err = NULL;
    int rc = 0;
   
    cJSON *json_info = cJSON_CreateObject();
    cJSON *users = cJSON_CreateArray();
    cJSON *user = cJSON_CreateObject();
    cJSON *message = cJSON_CreateObject();

    asprintf(&query, "SELECT count(*) FROM user_chat WHERE id_user = %i AND id_chat = %i;",
             MX_VINT(jsn, "id_sender"), MX_VINT(jsn, "id_chat"));
    rc = sqlite3_exec(db, query, callback, NULL, &err);
    if (mx_check(rc, err) != SQLITE_OK)
    {
        MX_SET_STATUS(jsn, 0);
        MX_SET_MESSAGE_STATUS(jsn, "Вы не состоите в этом чате");
    }
    else
    {
        free(query);
        
        asprintf(&query, "INSERT INTO message_table(id_chat, id_sender, message, send_at)"
                         "VALUES(%i, %i, '%s', datetime('now', 'localtime'));",
                 MX_VINT(jsn, "id_chat"), MX_VINT(jsn, "id_sender"), MX_VSTR(jsn, "message"));
        rc = sqlite3_exec(db, query, NULL, NULL, &err);
        if (mx_check(rc, err) != SQLITE_OK)
        {
            MX_SET_STATUS(jsn, 0);
            MX_SET_MESSAGE_STATUS(jsn, "Не удалось отправить сообщение!");
            free(query);
        }
        else
        {
            int id_message = sqlite3_last_insert_rowid(db);
            
            free(query);
            asprintf(&query, "SELECT id_user FROM user_chat INNER JOIN chat  \
                ON user_chat.id_chat = chat.id_chat \
                WHERE  user_chat.id_chat = %i;",
                     MX_VINT(jsn, "id_chat"));
            rc = sqlite3_exec(db, query, callback2, users, &err);
            if (mx_check(rc, err) != SQLITE_OK)
            {
                MX_SET_STATUS(jsn, 0);
                MX_SET_MESSAGE_STATUS(jsn, "Не удалось отправить сообщение2!");
            }
            else
            {
                MX_SET_STATUS(jsn, 1);
                MX_SET_MESSAGE_STATUS(jsn, "Сообщение отправлено!");
                set_message_status(db, MX_VINT(jsn, "id_chat"), cJSON_GetArraySize(users));
                cJSON_AddNumberToObject(message, "id_message", id_message);
                cJSON_AddStringToObject(message, "message", MX_VSTR(jsn, "message"));
                cJSON_AddNumberToObject(message, "id_chat", MX_VINT(jsn, "id_chat"));
                get_message_time(db, message);

                cJSON_AddNumberToObject(user, "id_user", MX_VINT(jsn, "id_sender"));
                get_sender_info(db, user);
                cJSON_AddItemToObject(json_info, "user", cJSON_Duplicate(user, 1));
                cJSON_AddItemToObject(json_info, "users", cJSON_Duplicate(users, 1));
                cJSON_AddItemToObject(json_info, "message", cJSON_Duplicate(message, 1));
                //cJSON_AddNumberToObject(json_info, "id_chat", MX_VINT(jsn, "id_chat"));
                cJSON_AddItemToObject(jsn, "json_info", cJSON_Duplicate(json_info, 1));
            }
        }
    }

    cJSON_DeleteItemFromObject(jsn, "id_chat");
    cJSON_DeleteItemFromObject(jsn, "id_sender");
    cJSON_DeleteItemFromObject(jsn, "message");
    //cJSON_DeleteItemFromObject(jsn, "count");
    cJSON_Delete(json_info);
    cJSON_Delete(user);
    cJSON_Delete(users);
    cJSON_Delete(message);
    free(query);
    return jsn;
}
