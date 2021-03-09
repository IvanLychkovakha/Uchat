#include "uchat.h"
#include "structs.h" 
#include "gui.h"


#define SIZE_CHAT(x) (sizeof(x)/ sizeof((x)[0]))   

/////////////net_functions/////////////
int Socket(int domain, int type, int protocol);
int Connect(int sockfg, const struct sockaddr *address, socklen_t address_len);
int Inet_pton(int af, const char* restrict src, void* restrict dst);
char* Read_socket(int socketfd);
void Write_socket(int socketfd, char* line);


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

/////////////send_requests/////////////
void send_request(int socketfd, char* json, enum request_type req);
void registr_user(struct client_info* client, int socketfd, char *user_name, char* user_surname, char* user_password, char* phone, char* image, int* tagsid, int tags_length);
void login_user(int socketfd, char* phone, char* password);
void send_message(int socketfd, int id_user, int id_chat, char* message);
void edit_message(int socketfd, int id_message, char* message,int id_sender, int id_chat);
void delete_message(int socketfd,int id_chat, int id_message);
void create_chat(int socketfd, char* chat_name, int id_creater, int type, char* image, int* participants, int parts_length);
void search_by_tag_id(int socketfd, int id_tag, int id_user);
void get_chat_info(int socketfd, int id_chat, int id_user);
void delete_chat(int socketfd, int id_chat, int chat_status);
void get_all_tags(int socketfd);

void add_member_by_id(int socketfd, int id_user, int id_chat);

void print_all_users(struct client_info* client);

/////////////respons_handle_funcs/////////////
struct respons* receive_respons(int socketfd);

void respons_handler(struct client_info* client, struct respons* resp);

struct client_info *client;
t_uchat_bases *widgets;
//wig_log * widgets;

