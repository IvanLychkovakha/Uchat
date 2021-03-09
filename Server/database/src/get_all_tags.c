#include "../inc/database.h"

static int callback(void *data, int argc, char **argv, char **cols)
{
    cJSON *json_row = cJSON_CreateObject();
    cJSON_AddNumberToObject(json_row, cols[0], atoi(argv[0]));
    cJSON_AddStringToObject(json_row, cols[1], argv[1]);

    cJSON_AddItemToArray(data, cJSON_Duplicate(json_row, 1));
    cJSON_Delete(json_row);

    return 0;
}
cJSON *mx_get_all_tags(sqlite3 *db, cJSON *jsn)
{

    char *query = NULL;
    char *err = NULL;
    int rc = 0;
    cJSON *json_info = cJSON_CreateObject();
    cJSON *tags = cJSON_CreateArray();
    asprintf(&query, "SELECT * FROM tags;");
    rc = sqlite3_exec(db, query, callback, tags, &err);
    if (mx_check(rc, err) != SQLITE_OK)
    {
        MX_SET_STATUS(jsn, 0);
        MX_SET_MESSAGE_STATUS(jsn, "Не удалось получить теги");
    }
    else
    {
        MX_SET_STATUS(jsn, 1);
        MX_SET_MESSAGE_STATUS(jsn, "Теги получены");
        cJSON_AddItemToObject(json_info, "tags", cJSON_Duplicate(tags, 1));
        cJSON_AddItemToObject(jsn, "json_info", cJSON_Duplicate(json_info, 1));
    }
    cJSON_Delete(tags);
    cJSON_Delete(json_info);
    free(query);
    return jsn;
}
