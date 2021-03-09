#include"server/inc/server.h"

#define LOG_FILE    "/FileLog.log"
#define FILE_MODE   "a+"
#define SERVER_UP   "Server up and running"

int main(int argc, char *argv[]) {
    t_server server;
    char *log_path;
    char *root_path = getenv("PWD");

    //Create daemon server
    create_server(&server, argc, argv[1]);
    

    //create path for LOG_FILE
    log_path = mx_strjoin(root_path, LOG_FILE);
    create_log(&server.log, log_path, FILE_MODE);
    free(log_path);

    //NUll clients
    server.list = NULL;

    //Server up logging
    logging(&server.log, INFO, NULL, SERVER_UP);

    //main server loop
    loop_server(&server);

    return EXIT_SUCCESS;
}

/*int main() {
    t_list_of_clients *list = NULL;
    t_client client;

    remove_client(&list, 124214124);
    while(list) {
        printf("%u\n", (unsigned int)(list));
        list = list->next;
    }
}*/
