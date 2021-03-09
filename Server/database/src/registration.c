#include "../inc/database.h"

static int callback(void *data, int argc, char **argv, char **cols)
{
    return strcmp(argv[0], "0") != 0;
}
static int callback2(void *data, int argc, char **argv, char **cols)
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
static int callback5(void *data, int argc, char **argv, char **cols)
{
    cJSON *json_row = cJSON_CreateObject();
    cJSON_AddNumberToObject(json_row, cols[0],atoi(argv[0]));
    cJSON_AddStringToObject(json_row, cols[1],argv[1]);
    cJSON_AddItemToArray(data,cJSON_Duplicate(json_row, 1));
    cJSON_Delete(json_row); 

    return 0;
}
static void get_user(sqlite3 *db, cJSON *user)
{
    char *query = NULL;
    char *err = NULL;
    int rc = 0;

    asprintf(&query, "SELECT count(*), image, user_name, user_surname, phone FROM user_table WHERE id_user = '%i'",
             MX_VINT(user, "id_user"));
    rc = sqlite3_exec(db, query, callback2, user, &err);
    free(query);
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
                    WHERE user_tag.id_user = '%d';",

                    id_user);
    rc = sqlite3_exec(db, query, callback5, tags, &err);
    cJSON_AddItemToObject(json_info, "tags", cJSON_Duplicate(tags, 1));
    cJSON_Delete(tags);
    //free(phone);
    free(query);
}
static void add_tags(sqlite3 *db, cJSON *jsn, int id_user)
{
    char *query = NULL;
    char *err = NULL;
    int rc = 0;
    cJSON *tags = cJSON_GetObjectItem(jsn, "tags");
    
    for (int i = 0; i < cJSON_GetArraySize(tags); i++)
    {
        //printf("id_tag:%d\n", cJSON_GetArrayItem(tags, i)->valueint);
        asprintf(&query, "INSERT INTO user_tag(id_user, id_tag) VALUES('%d', '%d');",id_user,cJSON_GetArrayItem(tags, i)->valueint);
        rc = sqlite3_exec(db, query, NULL, NULL, &err);
        free(query);
    }
}
static void accept_register(sqlite3 *db, cJSON *jsn, cJSON *json_info)
{
    char *query = NULL;
    char *err = NULL;
    int rc = 0;
    char * phone = MX_VSTR(jsn, "phone");
    char * two_last = strdup(&(phone[strlen(phone)-2])) ;
    printf("TWO LAST: %s\n", two_last);
    char * password = hash_password( MX_VSTR(jsn, "user_password"), two_last);
    asprintf(&query, "INSERT INTO user_table (phone, user_name, user_surname, user_password, image) VALUES ('%s', '%s', '%s','%s','%s');",
             phone, MX_VSTR(jsn, "user_name"), MX_VSTR(jsn, "user_surname"), password, MX_VSTR(jsn, "image"));
    rc = sqlite3_exec(db, query, NULL, NULL, &err);
    if (mx_check(rc, err) != SQLITE_OK)
    {
        MX_SET_STATUS(jsn, 0);
        MX_SET_MESSAGE_STATUS(jsn, "Введены некоректные данные");
    }
    else
    {
        MX_SET_STATUS(jsn, 1);
        MX_SET_MESSAGE_STATUS(jsn, "Пользователь успешно добавлен");
        int last_id = sqlite3_last_insert_rowid(db);
        add_tags(db, jsn, last_id);
        cJSON *user = cJSON_CreateObject();
        cJSON_AddNumberToObject(user, "id_user", last_id);
        get_user(db, user);
        get_tags(db, json_info, last_id);
        cJSON_AddItemToObject(json_info, "user", cJSON_Duplicate(user, 1));

        cJSON_AddItemToObject(jsn, "json_info", cJSON_Duplicate(json_info, 1));
        cJSON_Delete(user);
    }
    free(query);
}
static void Del_extra_info(cJSON *jsn)
{
    cJSON_DeleteItemFromObject(jsn, "phone");
    cJSON_DeleteItemFromObject(jsn, "user_password");
    cJSON_DeleteItemFromObject(jsn, "user_name");
    cJSON_DeleteItemFromObject(jsn, "user_surname");
    cJSON_DeleteItemFromObject(jsn, "image");
}
//phone, user_name, user_surname, user_password, image, tags(arr)
cJSON *mx_registration(sqlite3 *db, cJSON *jsn)
{
    char *query = NULL;
    char *err = NULL;
    int rc = 0;

    cJSON *json_info = cJSON_CreateObject();
    asprintf(&query, "SELECT count(*) FROM user_table WHERE phone = '%s';",
             MX_VSTR(jsn, "phone"));
    rc = sqlite3_exec(db, query, callback, NULL, &err);
    if (mx_check(rc, err) != SQLITE_OK)
    {
        MX_SET_STATUS(jsn, 0);
        MX_SET_MESSAGE_STATUS(jsn, "Этот пользователь уже существует!");
    }
    else
    {
        accept_register(db, jsn, json_info);
    }
    Del_extra_info(jsn);
    cJSON_Delete(json_info);
    free(query);
    return jsn;
}
