#include"../inc/server.h"

void send_response_to_client(t_list_of_clients *list, int *users, int len_array_users, char *response) {
    t_list_of_clients *list_ptr = list;
    while(list_ptr) {
        printf("YES\n");

        for(int i = 0; i < len_array_users; i++) {
            printf("client\n");
            printf("user_id: %d\n", users[i]);
            printf("list_user_id: %d\n", list_ptr->client.user_id);
            if(list_ptr->client.user_id == users[i])  {
                printf("response\n");
                printf("SENDING RESPONSE TO USER %d\n", users[i]);
                send(list_ptr->client.socket, response, strlen(response), 0);
                break;
            }
        }
        list_ptr = list_ptr->next;
    }
}
