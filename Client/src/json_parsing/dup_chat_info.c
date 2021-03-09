#include "client.h"


struct chat_info* dup_chat_info()
{
    struct chat_info* chat = (struct chat_info*) malloc(sizeof(struct chat_info));

    chat->id = -1;
    chat->name = NULL;
    chat->image = NULL;
    chat->last_message = NULL;
    chat->last_message_time = NULL;
    chat->type = -1;
    chat->messages = (struct message_info**) malloc(sizeof(struct message_info*));
    chat->messages[0] = NULL;
    chat->users = (struct user_info**) malloc(sizeof(struct user_info*));
    chat->users[0] = NULL;
    
    return chat;
}
