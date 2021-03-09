#include "../inc/database.h"

static int callback(void *data, int argc, char **argv, char **cols)
{
    cJSON *json_row = cJSON_CreateObject();
    cJSON_AddNumberToObject(json_row, cols[0], atoi(argv[0]));
    for (int i = 1; i < argc; i++)
    {
        cJSON_AddStringToObject(json_row, cols[i], argv[i]);
    }
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
//id_tag, id_user
cJSON *mx_search_by_tag_id(sqlite3 *db, cJSON *jsn)
{
    printf("FFFFFFFFfffffhfjdsffdsfD;LFL;FKLDSKFL;FKDS;LFKLFSD;");
    char *query = NULL;
    char *err = NULL;
    int rc = 0;
    cJSON *json_info = cJSON_CreateObject();
    cJSON *users = cJSON_CreateArray();
    asprintf(&query, "SELECT user_table.id_user, phone, user_name, user_surname, image FROM user_table\
                    JOIN user_tag ON user_table.id_user = user_tag.id_user\
                    JOIN tags ON user_tag.id_tag = tags.id_tag\
                    WHERE tags.id_tag = %i AND user_tag.id_user != %i;", MX_VINT(jsn, "id_tag"), MX_VINT(jsn,  "id_user"));
    rc = sqlite3_exec(db, query, callback, users, &err);
    if (mx_check(rc, err) != SQLITE_OK)
    {
        MX_SET_STATUS(jsn, 0);
        MX_SET_MESSAGE_STATUS(jsn, "Не удалось получить пользователей");
    }
    else
    {
        MX_SET_STATUS(jsn, 1);
        MX_SET_MESSAGE_STATUS(jsn, "Пользователи получены");
        for (int i = 0; i < cJSON_GetArraySize(users); i++)
        {
            cJSON * row = cJSON_GetArrayItem(users, i);

            get_tags(db, row, MX_VINT(row, "id_user"));

            
        }
         cJSON_AddItemToObject(json_info, "users", cJSON_Duplicate(users, 1));
        cJSON_AddItemToObject(jsn, "json_info", cJSON_Duplicate(json_info, 1));
    }
    cJSON_Delete(users);
    cJSON_Delete(json_info);
    free(query);
    return jsn;
}


