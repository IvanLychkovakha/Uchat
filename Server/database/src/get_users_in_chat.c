#include "../inc/database.h"

static int callback3(void *data, int argc, char **argv, char **cols)
{
    /*for(int i = 0; i < argc; i++){
        printf("%s:%s\n",cols[i],argv[i]);
    }*/
    cJSON *json_row = cJSON_CreateObject();
    cJSON_AddNumberToObject(json_row, cols[0], atoi(argv[0]));
    cJSON_AddItemToArray(data, cJSON_Duplicate(json_row, 1));
    cJSON_Delete(json_row);
    return 0;
}

void get_users_in_chat(sqlite3 *db, cJSON *json_info)
{
    char *query = NULL;
    char *err = NULL;
    int rc = 0;
    cJSON *users = cJSON_CreateArray();
    // printf("id:%i\n",MX_VINT(jsn, "id_chat") );
    asprintf(&query, "SELECT user_chat.id_user FROM user_chat \
                    INNER JOIN user_table ON user_chat.id_user = user_table.id_user \
                    WHERE id_chat = %i;",
             MX_VINT(json_info, "id_chat"));
    rc = sqlite3_exec(db, query, callback3, users, &err);

    cJSON_AddItemToObject(json_info, "users", cJSON_Duplicate(users, 1));

    cJSON_Delete(users);
    free(query);
}
