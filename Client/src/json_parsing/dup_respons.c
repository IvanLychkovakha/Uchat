#include "client.h"


struct respons* dup_respons()
{
    struct respons* resp = (struct respons*) malloc(sizeof(struct respons));

    resp->id = -1;
    resp->json_info = NULL;
    resp->message = NULL;
    char* json_info = NULL;
    return resp;
}
