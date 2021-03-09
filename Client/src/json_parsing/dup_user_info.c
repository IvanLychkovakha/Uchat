#include "client.h"


struct user_info* dup_user_info()
{
    struct user_info* user = (struct user_info*) malloc(sizeof(struct user_info));

    user->id = -1;

    user->tags = (struct tag_info**) malloc(sizeof(struct tag_info*));
    user->tags[0] = NULL;

    user->phone_number = NULL;
    user->name = NULL;
    user->lastname = NULL;
    user->image = NULL;

    return user;
}
