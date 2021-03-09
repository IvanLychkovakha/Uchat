#include"../inc/server.h"

#define USAGE           "usage: server [port].\n"
#define BAD_PORT        "bad port number %d.\n" 
#define SOCK_FAILED     "socket creation failed.\n"
#define BIND_FAILED     "bind failed.\n"
#define LISENT_FAILED   "listen failed.\n"
#define QLEN            5

void create_server(t_server *server, int argc, char *port) {
    if(argc != 2) {
        fprintf(stderr, USAGE);
        exit(EXIT_FAILURE);
    }

    server->port = atoi(port);

    if(server->port <= 0) {
        fprintf(stderr, BAD_PORT, server->port);
        exit(EXIT_FAILURE);
    }

    if((server->socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        fprintf(stderr, SOCK_FAILED);
    }

    server->addr.sin_family = AF_INET;
    server->addr.sin_port = htons(server->port);
    server->addr.sin_addr.s_addr = INADDR_ANY;    

    if(bind(server->socket, (t_sockaddr*)&server->addr, sizeof(server->addr)) < 0) {
        fprintf(stderr, BIND_FAILED);
        exit(1);
    }

    if(listen(server->socket, QLEN) < 0) {
        fprintf(stderr, LISENT_FAILED);
        exit(1);
    }
    
    fflush(stderr);
}
