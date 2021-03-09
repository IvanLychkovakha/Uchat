#define MX_VSTR(x, y) (cJSON_GetObjectItem(x, y)->valuestring)
#define MX_SET_STATUS(x, y) (cJSON_AddNumberToObject(x, "status", y))
#define MX_SET_MESSAGE_STATUS(x, y) (cJSON_AddStringToObject(x, "message_status", y))
#define MX_VINT(x, y) (cJSON_GetObjectItem(x, y)->valueint)
#define PATH_DB "Server/database/chat.db"
  
#include <sqlite3.h> 
#include "../../log/inc/log.h"
#include "../../json/inc/cJSON.h"

//hashfunc
char* hash_password(char* password, char *salt);
int prove_password(char* password, char* hashed_password);


cJSON *mx_registration(sqlite3 *db, cJSON *jsn);           // 0
cJSON *mx_auth(sqlite3 *db, cJSON *jsn);                   // 1


int mx_check(int rc, char *err);
void mx_init_db(sqlite3 **db);
cJSON *mx_get_all_users(sqlite3 *db, cJSON *jsn);//
cJSON *mx_request_controller(char *buffer, sqlite3 * db);

cJSON *mx_get_user_info(sqlite3 *db, cJSON *jsn);//
cJSON *mx_create_chat(sqlite3 *db, cJSON *jsn);//
cJSON *mx_send_message(sqlite3 *db, cJSON *jsn);//
cJSON *mx_delete_chat(sqlite3 *db, cJSON *jsn);//
cJSON *mx_delete_message(sqlite3 *db, cJSON *jsn);//
cJSON *mx_edit_message(sqlite3 *db, cJSON *jsn);//
cJSON *mx_add_member_by_id(sqlite3 *db, cJSON *jsn);//
cJSON *mx_get_chat_info(sqlite3 *db, cJSON *jsn);//
cJSON *mx_get_all_tags(sqlite3 *db, cJSON *jsn);
cJSON *mx_search_by_tag_id(sqlite3 *db, cJSON *jsn);
void get_users_in_chat(sqlite3 *db, cJSON *json_info);
//cJSON *mx_delete_member_from_chat(sqlite3 *db, cJSON *jsn);
//cJSON *mx_get_chats(sqlite3 *db, cJSON *jsn);
//cJSON *mx_get_messages(sqlite3 *db, cJSON *jsn);
//cJSON *mx_rename_chat(sqlite3 *db, cJSON *jsn);
//cJSON *mx_delete_user(sqlite3 *db, cJSON *jsn);
//cJSON *mx_set_message_status(sqlite3 *db, cJSON *jsn);
//cJSON *mx_get_message_status(sqlite3 *db, cJSON *jsn);
//cJSON *mx_get_users_in_chat(sqlite3 *db, cJSON *jsn);
//cJSON *mx_add_member_by_phone(sqlite3 *db, cJSON *jsn);
//cJSON *mx_set_user_photo(sqlite3 *db, cJSON *jsn);

//нужно добавить возможность создавать чат на несколько пользователей, реализовать недостоющие запросы
enum request_type
{
    REGISTR,
    LOGIN,
    SEND_MESSAGE,
    EDIT_MESSAGE,
    DELETE_MESSAGE,
    CREATE_CHAT,
    ADD_MEMBER_BY_ID,
    DELETE_CHAT,
    GET_USER_INFO,
    GET_CHAT_INFO,
    GET_ALL_USERS,
    GET_ALL_TAGS,
    SEARCH_USERS_BY_TAG_ID
}req;
