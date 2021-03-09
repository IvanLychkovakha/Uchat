#include "../inc/database.h"

cJSON *mx_request_controller(char *buffer, sqlite3 * db)
{
    cJSON *jsn = cJSON_Parse(buffer);


    int type = MX_VINT(jsn, "json_type");

    if (type == REGISTR)
        jsn = mx_registration(db, jsn);
    else if(type == LOGIN)
        jsn = mx_auth(db, jsn);
    else if(type == GET_USER_INFO)
        jsn = mx_get_user_info(db, jsn);
    else if(type == SEND_MESSAGE)
        jsn = mx_send_message(db, jsn);
    else if(type == EDIT_MESSAGE)
        jsn = mx_edit_message( db,jsn);
    else if(type == DELETE_MESSAGE)
        jsn = mx_delete_message(db, jsn);
    else if(type == CREATE_CHAT)
        jsn = mx_create_chat(db, jsn);
    else if(type == DELETE_CHAT)
        jsn = mx_delete_chat(db, jsn);
    else if(type == ADD_MEMBER_BY_ID)
        jsn = mx_add_member_by_id(db, jsn);
    else if(type == DELETE_CHAT)
        jsn = mx_delete_chat(db, jsn);
    else if(type == GET_USER_INFO)
        jsn = mx_get_user_info(db, jsn);
    else if(type == GET_CHAT_INFO)
        jsn = mx_get_chat_info(db, jsn);
    else if(type == GET_ALL_USERS)
        jsn = mx_get_all_users(db, jsn);
    else if(type == GET_ALL_TAGS)
        jsn = mx_get_all_tags(db, jsn);
    else if(type == SEARCH_USERS_BY_TAG_ID)
        jsn = mx_search_by_tag_id(db, jsn);
    //write(1, cJSON_Print(jsn), strlen(cJSON_Print(jsn)));
    return jsn;    
}
