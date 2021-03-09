#include"../inc/server.h"
#include"signal.h"
t_server *server;
pthread_mutex_t lock;

#define BUF 1024
#define STATUS "status"
#define TYPE "json_type"

static char* recv_all(t_list_of_clients *current) {
    int bufsiz = BUF;
    char *request = mx_strnew(bufsiz);

    for(int i = 0, data_len = 0; (data_len = recv(current->client.socket, &request[i], bufsiz, 0)) > 0; i += bufsiz) {
        if(data_len == bufsiz) {
                request = realloc(request, strlen(request) + bufsiz);
                bzero(&request[strlen(request)], bufsiz);
        }
        else {
            break;
        }
    }

    return request;
}

static void mutex_add_client(t_client client, t_list_of_clients **current){
    pthread_mutex_lock(&lock);
    add_client(&server->list, client);
    *current = server->list;
    logging(&server->log, INFO, NULL, "client connected with socket_fd %d",(*current)->client.socket);
    pthread_mutex_unlock(&lock);
}

static void mutex_remove_client(t_client client, t_list_of_clients **current){
    pthread_mutex_lock(&lock);
    logging(&server->log, INFO, NULL, "client disconected with socket_fd %d",(*current)->client.socket);
    remove_client(&server->list, (unsigned int)(*current));
    *current = NULL;
    pthread_mutex_unlock(&lock);
}

static bool init_user_id(t_list_of_clients * current, cJSON * json_response){
    printf("INIT USER: %s\n", cJSON_Print(json_response));
    fflush(stdout);
    if(current->client.user_id == -1) {
        cJSON * user =  cJSON_GetObjectItem( cJSON_GetObjectItem(json_response, "json_info") , "user" );
        current->client.user_id = cJSON_GetObjectItem(user, "id_user")->valueint;
        logging(&server->log, INFO, NULL, "client with socket_fd %d, set user_id %d", current->client.socket, current->client.user_id);
        return true;
    }
    return false;
}

static void free_step(cJSON ** json_response, char **request){
    cJSON_Delete(*json_response);
    free(*request);
}

static void update_all_users(cJSON * json_response){
    cJSON *users = cJSON_GetObjectItem(cJSON_GetObjectItem(json_response, "json_info"), "users");
    int size = cJSON_GetArraySize(users);
    printf("SIZE: %d\n", size);
    int int_users[size];
    for(int i = 0; i < size; i++){
        int_users[i] = cJSON_GetArrayItem(cJSON_GetArrayItem(users, i), 0)->valueint;
        printf("USER_ID: %d\n", int_users[i]);
    }
    cJSON_DeleteItemFromObject(cJSON_GetObjectItem(json_response, "json_info"), "users");
    char *response = cJSON_Print(json_response);
    send_response_to_client(server->list, int_users, size, response);
}

static void update_one_user(cJSON * json_response){
    t_list_of_clients * ptr = server->list;
    int status = MX_VINT(json_response, STATUS);
    if(status == 1){
        int id_user = cJSON_GetObjectItem(cJSON_GetObjectItem(json_response, "json_info"), "id_user")->valueint;
        //cJSON_DeleteItemFromObject(cJSON_GetObjectItem(json_response, "json_info"), "id_user");
        while(ptr){
            if(ptr->client.user_id == id_user){
                send(ptr->client.socket, cJSON_Print(json_response), strlen(cJSON_Print(json_response)), 0);
                break;
            }
            ptr = ptr->next;
        }
    }
}

static void mx_response_controller(cJSON * json_response, t_list_of_clients ** current){
    int type = MX_VINT(json_response, TYPE);                    // тип запиту
    int status = MX_VINT(json_response, STATUS);                // 
    char * char_json_response = cJSON_Print(json_response);


    if(type < 2){
        if(status)
            init_user_id(*current, json_response);
        send((*current)->client.socket, char_json_response, strlen(char_json_response), 0);
    }
    else if((type == SEND_MESSAGE || type == EDIT_MESSAGE || type == DELETE_MESSAGE || type == DELETE_CHAT || type == CREATE_CHAT || type == ADD_MEMBER_BY_ID) && status){
        update_all_users(json_response);
    }
    else if(type == ADD_MEMBER_BY_ID && status)
        update_one_user(json_response);
    else
        send((*current)->client.socket, char_json_response, strlen(char_json_response), 0);

    free(char_json_response);
    
}


static void* request_handler(void *arg) {
    t_client client = *((t_client*)arg);
    char connect = '\0';
    t_list_of_clients *current = NULL;
    cJSON *json_response = NULL;
    char *request = NULL;
    int x = 0;

    mutex_add_client(client, &current);

    while((x = recv(current->client.socket, &connect, 1, MSG_PEEK)) > 0) {

        request = recv_all(current);
        
        printf("%s\n", request);

        json_response  = mx_request_controller(request, server->db);

        printf("%s\n", cJSON_Print(json_response));

        mx_response_controller(json_response, &current);

        free_step(&json_response, &request);

    }
    mutex_remove_client(client, &current);

    pthread_exit(0);
}

void sign(int signal) {
    close(server->socket);
    logging(&server->log, FATAL, NULL, "Server CTRL+C");
    kill(getpid(), SIGINT);
    exit(-3);
}
void loop_server(t_server *_server) {
    t_client client;
    pthread_t tid;
    int len_addr = sizeof(t_sockaddr_in);
    server = _server;
    mx_init_db(&server->db);
    signal(SIGINT, sign);
    //daemonize_server();
    while(1) {
        if((client.socket = accept(server->socket, 
                            (t_sockaddr*)&client.addr,
                            (socklen_t*)&len_addr)) < 0) {
                                ;//T0DO
                            }
        pthread_create(&tid, NULL, request_handler, (void*)&client);
    }

    //pthread_mutex_destroy(&lock);
}
