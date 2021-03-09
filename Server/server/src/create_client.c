#include"../inc/server.h"

t_list_of_clients *create_client(t_client client) {
    t_list_of_clients *node = malloc(sizeof(t_list_of_clients));

    node->client = client;
    node->client.user_id = -1;
    node->next = NULL;

    return node;
}
