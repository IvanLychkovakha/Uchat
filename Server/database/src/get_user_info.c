#include "../inc/database.h"


static int callback(void *data, int argc, char **argv, char **cols) {
    if (!(strcmp(argv[0], "1"))) {
        
        for(int i = 1; i < argc; i++){
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
//id_user
cJSON *mx_get_user_info(sqlite3 *db, cJSON *jsn) {
    char *query = NULL;
    char *err = NULL;
    int rc = 0;
    cJSON *json_info = cJSON_CreateObject();
    cJSON *user = cJSON_CreateObject();
    //printf("id:%i\n",MX_VINT(jsn, "id_user") );
    asprintf(&query, "SELECT count(*), image, user_name, user_surname, phone FROM user_table WHERE id_user = '%i'", 
            MX_VINT(jsn, "id_user"));
    rc = sqlite3_exec(db, query, callback, user, &err);

    if (mx_check(rc, err) != SQLITE_OK){
        MX_SET_STATUS(jsn, 0);
        MX_SET_MESSAGE_STATUS(jsn, "Пользователь не найден");
       
    }
    else
    {
        MX_SET_STATUS(jsn, 1);
        MX_SET_MESSAGE_STATUS(jsn, "Данные получены");
        get_tags(db, json_info, MX_VINT(jsn, "id_user"));
        cJSON_AddNumberToObject(user, "id_user",  MX_VINT(jsn, "id_user"));
        cJSON_AddItemToObject(json_info, "user", cJSON_Duplicate(user,1));
        cJSON_AddItemToObject(jsn, "json_info", cJSON_Duplicate(json_info,1));
    }
    cJSON_DeleteItemFromObject(jsn, "id_user");
    cJSON_Delete(json_info);
    cJSON_Delete(user);
    free(query);
    
    return jsn;
}
