#include "dictionary.h"

struct user_info
{
    int id;
    char* phone_number;
    char* name;
    char* lastname;
    char* image;    
    struct tag_info** tags;
};

struct chat_info
{
    int id;
    char* name;
    int type;
    char* image;
    char* last_message;
    char* last_message_time;
    struct message_info** messages;
    struct user_info** users;
};

struct message_info
{
    int id;
    int chatid;
    char* text;
    char* date;
    int is_read;
    struct user_info* sender;
};

struct respons
{
    int id;
    int json_type;
    char* message;
    char* json_info;
};

struct client_info
{
    int socketfd;
    int status;
    struct user_info* current_user;
    struct chat_info** chats;
    struct user_info** searced_users;
    struct tag_info** tags;
    int last_id_chat;
    int who_push;
    int who_add;
    struct callback_funcs* callbacks;
};

struct tag_info
{
    int id;
    char *name;
};

struct callback_funcs
{
    void (*get_register_user)(struct client_info* client);
    void (*get_login_user)();
    void (*get_user_info)(struct client_info* client);
    void (*get_send_message)(struct client_info* client);
    void (*get_chat_info)(struct client_info* client);
    void (*get_edit_message)(struct client_info* client);
    void (*get_delete_message)(struct client_info* client);
    void (*get_create_chat)(struct client_info* client);
    void (*get_add_member_by_id)(struct client_info* client);
    void (*get_delete_chat)(struct client_info* client);
    void (*get_search_users_by_tag_id)(struct client_info* client);
    void (*get_all_tags)(struct client_info* client);
};


struct client_info* dup_client_info();
struct callback_funcs* dup_callbacks();

struct tag_info* dup_tag_info();
void set_tag_info_from_json(const char *json, struct tag_info *tag);

struct user_info* dup_user_info();
void set_user_info_from_json(const char *json, struct user_info *user);

struct chat_info* dup_chat_info();
void set_chat_info_from_json(const char *json, struct chat_info *chat);

struct message_info* dup_message_info();
void set_message_info_from_json(const char *json, struct message_info *message);

struct respons* dup_respons();
void set_respons_from_json(const char *json, struct respons *resp);
