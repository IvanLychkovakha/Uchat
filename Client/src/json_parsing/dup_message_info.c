#include "client.h"


struct message_info* dup_message_info()
{
    struct message_info* message = (struct message_info*) malloc(sizeof(struct message_info));

    message->id = -1;
    message->chatid = -1;
    message->is_read = -1;
    message->text = NULL;
    message->date = NULL;

    message->sender = dup_user_info();

    return message;
}
