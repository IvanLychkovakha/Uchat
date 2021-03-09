#include "client.h"
#include "pthread.h"

static void after_login();
static void change_status();
static void get_send();
static void search_user_tag();
static void add_chat();
void get_all_tas(struct client_info* client)
{
    printf("tags:\n");
    for (int i = 0; client->tags[i] != NULL; i++)
    {
        printf("tag: \n");
        printf("id = %d\n", client->tags[i]->id);
        printf("name = %s\n", client->tags[i]->name);
        fflush(stdout);
    }
}

void print_users_with_tag(struct client_info* client)
{
    printf("searched_users:\n");
    for (int i = 0; client->searced_users[i] != NULL; i++)
    {
            printf("user: \n");
            printf("id = %d\n", client->searced_users[i]->id);
            printf("name = %s\n", client->searced_users[i]->name);
            printf("lastname = %s\n", client->searced_users[i]->lastname);
            printf("image = %s\n", client->searced_users[i]->image);
            printf("phone number = %s\n", client->searced_users[i]->phone_number);
            fflush(stdout);

            for (int j = 0; client->searced_users[i]->tags[j]; j++)
            {
                printf("tag: \n");
                printf("id = %d\n", client->searced_users[i]->tags[j]->id);
                printf("name = %s\n", client->searced_users[i]->tags[j]->name);
                fflush(stdout);
            }
            fflush(stdout);
    }
    
}

void print_all_users(struct client_info* client)
{
    printf("client: \n");
    printf("current user: \n");
    printf("user id = %d\n", client->current_user->id);
    printf("user name = %s\n", client->current_user->name);
    printf("user image = %s\n", client->current_user->image);
    printf("user surname = %s\n", client->current_user->lastname);
    printf("user phone = %s\n", client->current_user->phone_number);
    fflush(stdout);

    printf("tags\n");
    for(int i = 0; client->current_user->tags[i] != NULL; i++)
    {
        printf("tag id = %d\n", client->current_user->tags[i]->id);
        printf("tag name = %s\n", client->current_user->tags[i]->name);
        fflush(stdout);
    }

    printf("chats: \n");
    for(int i = 0; client->chats[i] != NULL; i++)
    {
        printf("chat: \n");
        fflush(stdout);
        printf(" id = %d\n", client->chats[i]->id);
        printf(" name = %s\n", client->chats[i]->name);
        printf(" image = %s\n", client->chats[i]->image);
        printf(" last message = %s\n", client->chats[i]->last_message);
        printf(" last message time = %s\n", client->chats[i]->last_message_time);
        printf(" type = %d\n", client->chats[i]->type);
        fflush(stdout);

        printf("messages: \n");
        for(int j = 0; client->chats[i]->messages[j] != NULL; j++)
        {
            printf("message: \n");
            printf(" id = %d\n", client->chats[i]->messages[j]->id);
            printf(" text = %s\n", client->chats[i]->messages[j]->text);
            printf(" date = %s\n", client->chats[i]->messages[j]->date);
            printf(" is read = %d\n", client->chats[i]->messages[j]->is_read);
            fflush(stdout);

            printf("sender: \n");
            printf("id = %d\n", client->chats[i]->messages[j]->sender->id);
            printf("name = %s\n", client->chats[i]->messages[j]->sender->name);
            printf("lastname = %s\n", client->chats[i]->messages[j]->sender->lastname);
            printf("image = %s\n", client->chats[i]->messages[j]->sender->image);
            printf("phone number = %s\n", client->chats[i]->messages[j]->sender->phone_number);
            fflush(stdout);

            printf("tags: \n");
            fflush(stdout);
            for (int k = 0; client->chats[i]->messages[j]->sender->tags[k] != NULL; k++)
            {
                printf("tag: \n");
                printf("id = %d\n", client->chats[i]->messages[j]->sender->tags[k]->id);
                printf("name = %s\n", client->chats[i]->messages[j]->sender->tags[k]->name);
                fflush(stdout);
            }
            fflush(stdout);
        }

        printf("users: \n");
        fflush(stdout);
        for(int j = 0; client->chats[i]->users[j] != NULL; j++)
        {        
            printf("user: \n");
            printf("id = %d\n", client->chats[i]->users[j]->id);
            printf("name = %s\n", client->chats[i]->users[j]->name);
            printf("lastname = %s\n", client->chats[i]->users[j]->lastname);
            printf("image = %s\n", client->chats[i]->users[j]->image);
            printf("phone number = %s\n", client->chats[i]->users[j]->phone_number);
            fflush(stdout);

            printf("tags: \n");
            fflush(stdout);
            for (int k = 0; client->chats[i]->users[j]->tags[k] != NULL; k++)
            {
                printf("tag: \n");
                printf("id = %d\n", client->chats[i]->users[j]->tags[k]->id);
                printf("name = %s\n", client->chats[i]->users[j]->tags[k]->name);
                fflush(stdout);
            }
            fflush(stdout);
        }
        fflush(stdout);
    }
    fflush(stdout);
}

static void* handle_respons(void *arg);

int main(int argc, char**  argv)
{
    int socketfd = 0;
    char *buff = (char*) malloc(1024);
    struct sockaddr_in serv_addr;

    if(argc != 3)
    {
        printf("\nError: not enough arguments. Use: <ip, port>\n");
        exit(EXIT_FAILURE);
    }

    memset(buff, '0', 1024);
    socketfd = Socket(AF_INET, SOCK_STREAM, 0);
    memset(&serv_addr, '0', sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(atoi(argv[2]));

    Inet_pton(AF_INET, argv[1], &serv_addr.sin_addr);

    client = dup_client_info();
    client->socketfd = socketfd;
    client->callbacks->get_add_member_by_id = &change_status;
    client->callbacks->get_chat_info = &change_status;
    client->callbacks->get_create_chat = &add_chat;
    client->callbacks->get_delete_chat = &change_status;
    client->callbacks->get_delete_message = &change_status;
    client->callbacks->get_edit_message = &change_status;
    client->callbacks->get_login_user = &change_status;
    client->callbacks->get_register_user = &change_status;
    client->callbacks->get_send_message = &get_send;
    client->callbacks->get_search_users_by_tag_id = &search_user_tag;
    client->callbacks->get_all_tags = &change_status;

    int con = Connect(socketfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr));

   /* int participants[2] = {16, 17};
    //int tags[2] = {1, 2};
    //registr_user(client,client->socketfd, "Timon", "Surikat", "surikat", "suricat", "No image", tags, 2);
    login_user(client->socketfd, "suricat", "suricat");
    sleep(3);
    int tags[2] = {2};
    //registr_user(client,client->socketfd, "Pumba", "Kaban", "kaban", "kaban", "No image", tags, 1);
    sleep(3);
    create_chat(client->socketfd, "Timka", 17, 1, "No image", participants, 2);
    sleep(3);
    send_message(client->socketfd, 17, 70, "Hello timkam");*/

    pthread_t tid;
    pthread_create(&tid, NULL, handle_respons, (void*)client);

    log_main_loop();
    uchat_base();
    return 0;
}


static void add_chat(){
    g_print("XXXxuis id \n");
    if(client->who_add == 1){
        client->status = 1;
        client->who_add = 0;
    }
    else{
        g_print("client->== id, %d",client->chats[0]->id);
        create_user_button(&(widgets->us_data->user_but),widgets->grid_v);
        g_signal_connect(widgets->us_data->user_but->user_info[0],"toggled",G_CALLBACK(user_toggled),widgets);
        gtk_grid_attach(GTK_GRID(widgets->grid_user_online),widgets->us_data->user_but->user_info[0],0, widgets->grid_v,1,1);
        widgets->grid_v += 1;
        create_us_chat(widgets);
        gtk_widget_show(widgets->us_data->user_ch->bases);
    }
}

static void search_user_tag(){
    if(widgets->us_data->search != NULL){
        widgets->us_data->search = search_clear_list(widgets->us_data->search);
    }
    for(int i = 0;  client->searced_users[i] != NULL; i++){
        search_list(&(widgets->us_data->search),i);
    }
}

static void get_send(){
    if(client->who_push){
        client->who_push = 0;
    }else{
        for(int i = 0; client->chats[i] != NULL; i++){
            if(client->chats[i]->id == client->last_id_chat){
                add_message(widgets,4,client->chats[i]->last_message,client->last_id_chat);
            }
        }
    }
}

static void change_status()
{
    client->status = 1;
}
static void after_login()
{
    //uchat_base();
    print_all_users(client);
    
}

static void* handle_respons(void *arg)
{
    struct client_info *client = ((struct client_info*)arg);

    while(1)
    {
        struct respons *resp = receive_respons(client->socketfd);

        respons_handler(client, resp);

        free(resp->json_info);
        free(resp->message);
        free(resp);
        resp = NULL;
    }
    pthread_exit(0);
}

