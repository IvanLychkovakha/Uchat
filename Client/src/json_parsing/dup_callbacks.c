#include "client.h"


struct callback_funcs* dup_callbacks()
{
    struct callback_funcs* callbacks = (struct callback_funcs*) malloc(sizeof(struct callback_funcs));

    callbacks->get_add_member_by_id = NULL;
    callbacks->get_chat_info = NULL;
    callbacks->get_create_chat = NULL;
    callbacks->get_delete_message = NULL;
    callbacks->get_delete_chat = NULL;
    callbacks->get_edit_message = NULL;
    callbacks->get_login_user = NULL;
    callbacks->get_register_user = NULL;
    callbacks->get_send_message = NULL;
    callbacks->get_all_tags = NULL;

    return callbacks;
}
