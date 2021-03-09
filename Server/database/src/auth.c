#include "../inc/database.h"

#define QUERY_GET_USER_PASSWORD "SELECT count(*), user_password, id_user FROM user_table WHERE phone = '%s'"
#define QUERY_GET_USER_ID "SELECT id_user FROM user_table WHERE phone = '%s';"
#define USER_ID "id_user"

static int add_user_id(void *data, int argc, char **argv, char **cols)
{
    cJSON_AddNumberToObject((cJSON *) data, USER_ID, atoi(argv[0]));
    return 0;
}
static int callback2(void *data, int argc, char **argv, char **cols)
{

    if (!(strcmp(argv[0], "1")))
    {

        cJSON *user = cJSON_CreateObject();
        cJSON_AddNumberToObject(user, cols[1], atoi(argv[1]));
        for (int i = 2; i < argc; i++)
        {
            cJSON_AddStringToObject(user, cols[i], argv[i]);
        }
        cJSON_AddItemToObject(data, "user", cJSON_Duplicate(user, 1));
        cJSON_Delete(user);

        return 0;
    }
    return 1;
}
static int callback3(void *data, int argc, char **argv, char **cols)
{
    /*for(int i = 0; i < argc; i++){
        printf("%s:%s\n",cols[i],argv[i]);
    }*/
    cJSON *json_row = cJSON_CreateObject();
    cJSON_AddNumberToObject(json_row, cols[0], atoi(argv[0]));
    cJSON_AddStringToObject(json_row, "chat_name", argv[1]);
    cJSON_AddNumberToObject(json_row, cols[2], atoi(argv[2]));
    cJSON_AddStringToObject(json_row, cols[3], argv[3]);
    cJSON_AddItemToArray(data, cJSON_Duplicate(json_row, 1));
    cJSON_Delete(json_row);
    return 0;
}

static int callback4(void *data, int argc, char **argv, char **cols)
{
    cJSON *json_row = cJSON_CreateObject();
    cJSON_AddNumberToObject(json_row, cols[0], atoi(argv[0]));
    cJSON_AddStringToObject(json_row, cols[1], argv[1]);
    cJSON_AddStringToObject(json_row, cols[2], argv[2]);
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
static void get_chats(sqlite3 *db, cJSON *chats, int id_user)
{
    char *query = NULL;
    char *err = NULL;
    int rc = 0;

    cJSON *chats_with_last_message = cJSON_CreateArray();
    asprintf(&query, "SELECT user_chat.id_chat, private_chat.chat_p_name, chat_status, chat_image \
                    FROM user_chat\
                    JOIN private_chat ON user_chat.id_chat = private_chat. id_chat\
                    WHERE user_chat.id_user = %i\
                    GROUP BY user_chat.id_chat\
                    UNION\
                    SELECT user_chat.id_chat, group_chat.chat_g_name, chat_status, chat_image\
                    FROM user_chat\
                    JOIN group_chat ON user_chat.id_chat = group_chat.id_chat\
                    WHERE user_chat.id_user = %i\
                    GROUP BY user_chat.id_chat",
             id_user, id_user);
    rc = sqlite3_exec(db, query, callback3, chats, &err);
    free(query);
    asprintf(&query, "SELECT chat.id_chat, message, send_at\
                    FROM (SELECT id_chat, message, send_at \
                    FROM message_table \
                    ORDER BY send_at DESC) AS T JOIN chat ON T.id_chat = chat.id_chat\
                    JOIN user_chat ON chat.id_chat = user_chat.id_chat\
                    JOIN private_chat ON user_chat.id_chat = private_chat. id_chat\
                    WHERE user_chat.id_user = %i\
                    GROUP BY chat.id_chat\
                    UNION\
                    SELECT chat.id_chat, message, send_at\
                    FROM (SELECT id_chat, message, send_at\
                    FROM message_table\
                    ORDER BY send_at DESC) AS T JOIN chat ON T.id_chat = chat.id_chat\
                    JOIN user_chat ON chat.id_chat = user_chat.id_chat\
                    JOIN group_chat ON user_chat.id_chat = group_chat.id_chat\
                    WHERE user_chat.id_user = %i\
                    GROUP BY chat.id_chat\
                    ORDER BY send_at DESC",
             id_user, id_user);
    rc = sqlite3_exec(db, query, callback4, chats_with_last_message, &err);
    free(query);

    for (int i = 0; i < cJSON_GetArraySize(chats); i++)
    {
        bool check = false;
        cJSON *json_row = cJSON_GetArrayItem(chats, i);
        for (int j = 0; j < cJSON_GetArraySize(chats_with_last_message); j++)
        {
            cJSON *json_row_with_messages = cJSON_GetArrayItem(chats_with_last_message, j);
            if (MX_VINT(json_row, "id_chat") == MX_VINT(json_row_with_messages, "id_chat"))
            {
                check = true;
                cJSON_AddStringToObject(json_row, "message", MX_VSTR(json_row_with_messages, "message"));
                cJSON_AddStringToObject(json_row, "send_at", MX_VSTR(json_row_with_messages, "send_at"));
            }
        }
        if (!check)
        {
            cJSON_AddStringToObject(json_row, "message", "");
            cJSON_AddStringToObject(json_row, "send_at", "");
        }
    }
    cJSON_Delete(chats_with_last_message);
    printf("%s\n", cJSON_Print(chats));
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
static void accept_auth(sqlite3 *db, cJSON *jsn, cJSON *json_info)
{
    char *query = NULL;
    char *err = NULL;
    int rc = 0;
    char *phone = MX_VSTR(jsn, "phone");
    asprintf(&query, "SELECT count(*), id_user, image, user_name, user_surname, phone FROM user_table WHERE phone = '%s'",
             phone);
    rc = sqlite3_exec(db, query, callback2, json_info, &err);
    //free(phone);
    free(query);
}


static int check_password(void *data, int argc, char **argv, char **cols)
{
    if(atoi(argv[0]) != 0){
        printf("PASSWORD: %s\n", (char *) data);
        printf("HASHED_PASSWORD: %s\n", argv[1]);
        int result = prove_password((char *) data, argv[1]);
        printf("CHECK RESULT: %d\n", result);
        return prove_password((char *) data, argv[1]);   
    }
    return 1;
}

// phone,user_password
cJSON *mx_auth(sqlite3 *db, cJSON *jsn)
{
    char *query = NULL;
    char *err = NULL;
    int rc = 0;
    cJSON *json_info = cJSON_CreateObject();
    cJSON *chats = cJSON_CreateArray();
    char *phone = MX_VSTR(jsn, "phone");
    char *user_password = MX_VSTR(jsn, "user_password");
    
    asprintf(&query, QUERY_GET_USER_PASSWORD, phone);
    rc = sqlite3_exec(db, query, check_password, user_password, &err);
    printf("rc = %d\n", rc);

    if (mx_check(rc, err) != SQLITE_OK)
    {
        MX_SET_STATUS(jsn, 0);
        MX_SET_MESSAGE_STATUS(jsn, "Phone or password is incorrect");
    }
    else
    {

        free(query);
        asprintf(&query, QUERY_GET_USER_ID, phone);
        rc = sqlite3_exec(db, query, add_user_id, jsn, &err);

        MX_SET_STATUS(jsn, 1);
        MX_SET_MESSAGE_STATUS(jsn, "Authorization was successful");

        accept_auth(db, jsn, json_info);
        
        get_chats(db, chats, MX_VINT(jsn, "id_user"));
        get_tags(db, json_info,MX_VINT(jsn, "id_user") );
        cJSON_AddItemToObject(json_info, "chats", cJSON_Duplicate(chats, 1));
        cJSON_AddItemToObject(jsn, "json_info", cJSON_Duplicate(json_info, 1));
    }
    cJSON_DeleteItemFromObject(jsn, "user_password");
    cJSON_DeleteItemFromObject(jsn, "id_user");
    cJSON_DeleteItemFromObject(jsn, "phone");
    cJSON_Delete(json_info);
    cJSON_Delete(chats);
    //free(phone);
    //free(user_password);
    free(query);
    return jsn;
}
