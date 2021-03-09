#include "client.h"


struct client_info* dup_client_info()
{
    struct client_info *client = (struct client_info *) malloc(sizeof(struct client_info));

    client->socketfd = -1;
    client->current_user = dup_user_info();
    client->status = 0;

    client->tags = (struct tag_info**) malloc(sizeof(struct tag_info*));
    client->tags[0] = NULL;

    client->chats = (struct chat_info**) malloc(sizeof(struct chat_info*));
    client->chats[0] = NULL;

    client->searced_users = (struct user_info**) malloc(sizeof(struct user_info*));
    client->searced_users[0] = NULL;

    client->tags = (struct tag_info**) malloc(sizeof(struct tag_info*));
    client->tags[0] = NULL;
    client->last_id_chat = -1;
    client->who_push = 0;
    client->who_add = 0;
    client->callbacks = dup_callbacks();

    return client;
}

