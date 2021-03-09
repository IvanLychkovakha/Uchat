#ifndef SERVER
#define SERVER

/*log содержит libmx со всеми библами и функциями
#include"../../libmx/inc/libmx.h"
*/
#include "../../log/inc/log.h"
#include "../../database/inc/database.h"

typedef struct sockaddr_in t_sockaddr_in;
typedef struct sockaddr t_sockaddr;

typedef struct s_client {
    int socket;
    int user_id;
    t_sockaddr_in addr;
} t_client;

typedef struct s_list_of_clients {
    t_client client;
    struct s_list_of_clients *next;
} t_list_of_clients;

typedef struct s_server {
    int port;
    t_sockaddr_in addr;
    int socket;
    t_log log;
    t_list_of_clients *list;
    sqlite3 *db;
} t_server;

t_list_of_clients *create_client(t_client client);
void add_client(t_list_of_clients **list_of_clients, t_client client);
void remove_client(t_list_of_clients **list_of_clients, unsigned int addr);
void daemonize_server(void);
void create_server(t_server *server, int argc, char *port);
void loop_server(t_server *server);
void send_response_to_client(t_list_of_clients *list, int *users, int len_array_users, char *response);

#endif
