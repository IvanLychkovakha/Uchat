#include "client.h"

struct tag_info* dup_tag_info()
{
    struct tag_info* tag = (struct tag_info*) malloc(sizeof(struct tag_info));

    tag->id = -1;
    tag->name = NULL;

    return tag;
}
