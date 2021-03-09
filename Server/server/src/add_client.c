#include"../inc/server.h"

void add_client(t_list_of_clients **s_list_of_clients, t_client client) {
    t_list_of_clients *temp = *s_list_of_clients;
    *s_list_of_clients = create_client(client);
    (*s_list_of_clients)->next = temp;
}
