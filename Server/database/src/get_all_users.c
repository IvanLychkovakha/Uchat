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
cJSON *mx_get_all_users(sqlite3 *db, cJSON *jsn)
{

    char *query = NULL;
    char *err = NULL;
    int rc = 0;
    cJSON *json_info = cJSON_CreateObject();
    cJSON *users = cJSON_CreateArray();
    asprintf(&query, "SELECT id_user, phone, user_name, user_surname, image FROM user_table;");
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
        cJSON_AddItemToObject(json_info, "users", cJSON_Duplicate(users, 1));
        cJSON_AddItemToObject(jsn, "json_info", cJSON_Duplicate(json_info, 1));
    }
    cJSON_Delete(users);
    cJSON_Delete(json_info);
    free(query);
    return jsn;
}
