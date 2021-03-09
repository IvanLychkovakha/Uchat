#include "client.h"



t_user_search *search_clear_list(t_user_search *head){
while (head != NULL){
    t_user_search *p = head;
    gtk_container_remove(GTK_CONTAINER(widgets->grid_popov_users),p->togle_but);
    head = head -> next;
    g_slice_free(t_user_search,p);
}
return NULL;
}
