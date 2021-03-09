#include"../inc/server.h"

void remove_client(t_list_of_clients **list_of_clients, unsigned int addr) {
    t_list_of_clients *prev = NULL;
    t_list_of_clients *ptr = *list_of_clients;

    if(!ptr)
        return;

    /*if(!(*s_list_of_clients)->next) {
        free(*s_list_of_clients);
        *s_list_of_clients = NULL;
        return;
    }*/
    if(!ptr->next) {
        *list_of_clients = (*list_of_clients)->next;
        free(ptr);
        return;
    }
    while(ptr) {
        if((unsigned int)(ptr) == addr) {
            if(prev == NULL) {
                *list_of_clients = (*list_of_clients)->next;
                free(ptr);
                return;
            } else {
                prev->next = ptr->next;
                free(ptr);
                return;
            }
        }

        prev = ptr;
        ptr = ptr->next;
    }
}
