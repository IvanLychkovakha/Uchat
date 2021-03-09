#include "client.h"

#define BUFF_SIZE 1024

struct respons *receive_respons(int socketfd)
{
    int bufsiz = BUFF_SIZE;
    struct respons* resp = dup_respons();
    resp->json_type = -1;
    char *request = (char *)malloc(bufsiz + 1);

    for (int i = 0, data_len = 0; (data_len = recv(socketfd, &request[i], bufsiz, 0)) > 0; i += bufsiz)
    {
        if (data_len == bufsiz)
        {
            request = realloc(request, strlen(request) + bufsiz);
            bzero(&request[strlen(request)], bufsiz);
        }
        else
        {
            break;
        }
    }
    //printf("request:%s\n", request);

    printf("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
    set_respons_from_json(request, resp);

    free(request);
    return resp;
    /*size_t received_count;
    char *request = malloc(1);
    char *temp_req = malloc(sizeof(char) * BUFF_SIZE);
    bzero(temp_req, BUFF_SIZE);
    bzero(request, 1);
    struct respons* resp = dup_respons();
    resp->json_type = -1;
    size_t len = 0;

    while((received_count = recv(socketfd, temp_req, BUFF_SIZE, 0)) > 0) 
    {
        if(received_count == BUFF_SIZE)
        {
            len += BUFF_SIZE;

            request = (char*) realloc(request, len);
            request = mx_strjoin(request, temp_req);
            continue;
        }
        request = (char*) realloc(request, len);
        request = mx_strjoin(request, temp_req);
        printf("request:%s\n",request);

        set_respons_from_json(request, resp);
        free(request);
        return resp;
    }
    return resp;*/
}
