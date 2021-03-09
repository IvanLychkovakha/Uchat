#include "client.h"


void respons_handler(struct client_info* client, struct respons* resp)
{
    printf("Respons message: %s\n", resp->message);
    if(resp->id == 0)
    {
        client->status = -1;
        return;
    }
    switch(resp->json_type)
    {
        case REGISTR:
        {
            cJSON* user_json = cJSON_GetObjectItem(cJSON_Parse(resp->json_info), "user");
            cJSON* tags = cJSON_GetObjectItem(cJSON_Parse(resp->json_info), "tags");

            client->current_user->id = cJSON_GetObjectItem(user_json, "id_user")->valueint;

            int tags_length = cJSON_GetArraySize(tags);
            client->current_user->tags = (struct tag_info**) malloc(sizeof(struct tag_info*) * (tags_length + 1));
            client->current_user->tags[tags_length] = NULL;

            for(int i = 0; i < tags_length; i++)
            {
                client->current_user->tags[i] = dup_tag_info();
                set_tag_info_from_json(cJSON_Print(cJSON_GetArrayItem(tags, i)), client->current_user->tags[i]);
            }
            
            if(client->callbacks->get_register_user != NULL)
                client->callbacks->get_register_user(client);

            cJSON_Delete(user_json);
            break;
        }
        case LOGIN:
        {
            cJSON *user_json = cJSON_GetObjectItem(cJSON_Parse(resp->json_info), "user");
            set_user_info_from_json(cJSON_Print(user_json), client->current_user);

            cJSON *tags_json = cJSON_GetObjectItem(cJSON_Parse(resp->json_info), "tags");
            int tag_size = cJSON_GetArraySize(tags_json);
            client->current_user->tags = (struct tag_info**) realloc(client->current_user->tags, sizeof(struct tag_info*) * (tag_size + 1));
            client->current_user->tags[tag_size] = NULL;

            for(int i = 0; i < tag_size; i++)
            {
                client->current_user->tags[i] = dup_tag_info();
                set_tag_info_from_json(cJSON_Print(cJSON_GetArrayItem(tags_json, i)), client->current_user->tags[i]);
            }

            set_user_info_from_json(cJSON_Print(user_json), client->current_user);

            cJSON *chats_json = cJSON_GetObjectItem(cJSON_Parse(resp->json_info), "chats");
            int chat_size = cJSON_GetArraySize(chats_json);
            client->chats = (struct chat_info**) malloc(sizeof(struct chat_info*) * (chat_size + 1));
            client->chats[chat_size] = NULL;

            for(int i = 0 ; i < chat_size; i++)
            {
                client->chats[i] = dup_chat_info();
                set_chat_info_from_json(cJSON_Print(cJSON_GetArrayItem(chats_json, i)), client->chats[i]);
            }

            cJSON_Delete(user_json);
            cJSON_Delete(chats_json);
            cJSON_Delete(tags_json);

            client->status = 1;

            if(client->callbacks->get_login_user != NULL)
                client->callbacks->get_login_user();
        break;
        }
        case SEND_MESSAGE:
        {
            cJSON* message_json = cJSON_GetObjectItem(cJSON_Parse(resp->json_info), "message");
            int chatid = cJSON_GetObjectItem(message_json, "id_chat")->valueint;
            int messageid = cJSON_GetObjectItem(message_json, "id_message")->valueint;
            client->last_id_chat = chatid;
            cJSON* user_json = cJSON_GetObjectItem(cJSON_Parse(resp->json_info), "user");
            int userid = cJSON_GetObjectItem(user_json, "id_user")->valueint;
            fflush(stdout);

            for(int i = 0; client->chats[i] != NULL; i++)
            {
                if(client->chats[i]->id == chatid)
                {
                    int size = 0;
                    for(; client->chats[i]->messages[size] != NULL; size++){}


                    client->chats[i]->messages = (struct message_info**) realloc(client->chats[i]->messages, sizeof(struct message_info*) * (size + 2));
                    client->chats[i]->messages[size + 1] = NULL;
                    client->chats[i]->messages[size] = dup_message_info();

                    set_message_info_from_json(cJSON_Print(message_json), client->chats[i]->messages[size]);
                    client->chats[i]->last_message = client->chats[i]->messages[size]->text;
                    client->chats[i]->last_message_time = client->chats[i]->messages[size]->date;


                    for(int j = 0; client->chats[i]->users[j] != NULL; j++)
                    {
                        if(client->chats[i]->users[j]->id == userid)
                        {
                            client->chats[i]->messages[size]->sender = client->chats[i]->users[j];
                            break;
                        }
                    }
                    break;
                }
            }

            cJSON_Delete(message_json);
            cJSON_Delete(user_json);


            if(client->callbacks->get_send_message != NULL)
                client->callbacks->get_send_message(client);
            break;
        }
        case GET_CHAT_INFO:
        {        
            cJSON* messages_json = cJSON_GetObjectItem(cJSON_Parse(resp->json_info), "messages");
            int messages_size = cJSON_GetArraySize(messages_json);

            cJSON* users_json = cJSON_GetObjectItem(cJSON_Parse(resp->json_info), "users");
            int users_size = cJSON_GetArraySize(users_json);

            int chat_id = cJSON_GetObjectItem(cJSON_Parse(resp->json_info), "id_chat")->valueint;
            bool need_add = true;

            for(int k = 0; client->chats[k] != NULL; k++)
            {
                if(client->chats[k]->id == chat_id)
                {
                    for (int i = 0; client->chats[k]->users[i] != NULL; i++)
                    {
                        free(client->chats[k]->users[i]->image);
                        free(client->chats[k]->users[i]->lastname);
                        free(client->chats[k]->users[i]->name);
                        if(client->chats[k]->users[i]->phone_number != NULL)
                            free(client->chats[k]->users[i]->phone_number);
                    }
                    free(client->chats[k]->users);

                    client->chats[k]->users = (struct user_info**) malloc(sizeof(struct user_info*) * (users_size + 1));
                    client->chats[k]->users[users_size] = NULL;

                    for (int i = 0; i < users_size; i++)
                    {
                        cJSON* user_json = cJSON_GetArrayItem(users_json, i);

                        client->chats[k]->users[i] = dup_user_info();
                        set_user_info_from_json(cJSON_Print(user_json), client->chats[k]->users[i]);
                    }

                    for (int i = 0; client->chats[k]->messages[i] != NULL; i++)
                    {
                        free(client->chats[k]->messages[i]->date);
                        free(client->chats[k]->messages[i]->text);
                        free(client->chats[k]->messages[i]->sender);
                    }

                    free(client->chats[k]->messages);

                    client->chats[k]->messages = (struct message_info**) malloc(sizeof(struct message_info*) * (messages_size + 1));
                    client->chats[k]->messages[messages_size] = NULL;

                    for (int i = 0; i < messages_size; i++)
                    {
                        cJSON* message_json = cJSON_GetArrayItem(messages_json, i);

                        client->chats[k]->messages[i] = dup_message_info();

                        set_message_info_from_json(cJSON_Print(message_json), client->chats[k]->messages[i]);

                        int userid = cJSON_GetObjectItem(message_json, "id_sender")->valueint;

                        for(int j = 0; client->chats[k]->users[j] != NULL; j++)
                        {
                            if(client->chats[k]->users[j]->id == userid)
                            {
                                client->chats[k]->messages[i]->sender = client->chats[k]->users[j];
                                fflush(stdout);
                                break;
                            }
                        }
                    }
                    break;
                }
            }
            cJSON_Delete(messages_json);
            cJSON_Delete(users_json);

            if(client->callbacks->get_chat_info != NULL)
                client->callbacks->get_chat_info(client);
            break;
        }
        case EDIT_MESSAGE:
        {
            cJSON* message_json = cJSON_GetObjectItem(cJSON_Parse(resp->json_info), "message");
            int chatid = cJSON_GetObjectItem(message_json, "id_chat")->valueint;
            int messageid = cJSON_GetObjectItem(message_json, "id_message")->valueint;

            cJSON* user_json = cJSON_GetObjectItem(cJSON_Parse(resp->json_info), "user");

            for(int i = 0; client->chats[i] != NULL; i++)
            {
                if(client->chats[i]->id == chatid)
                {
                    for(int j = 0; client->chats[i]->messages[j] != NULL; j++)
                    {
                        if( client->chats[i]->messages[j]->id == messageid)
                        {
                            set_message_info_from_json(cJSON_Print(message_json), client->chats[i]->messages[j]);
                            set_user_info_from_json(cJSON_Print(user_json), client->chats[i]->messages[j]->sender);
                            break;
                        }
                    }
                }
            }

            cJSON_Delete(message_json);
            cJSON_Delete(user_json);

            if(client->callbacks->get_edit_message != NULL)
                client->callbacks->get_edit_message(client);
            break;
        }
        case DELETE_MESSAGE:
        {
            int chatid = cJSON_GetObjectItem(cJSON_Parse(resp->json_info), "id_chat")->valueint;
            int messageid = cJSON_GetObjectItem(cJSON_Parse(resp->json_info), "id_message")->valueint;

            for(int i = 0; client->chats[i] != NULL; i++)
            {
                if(client->chats[i]->id == chatid)
                {
                    printf("Chat finded\n");
                    fflush(stdout);

                    for(int j = 0; client->chats[i]->messages[j] != NULL; j++)//////////////////////////////////
                    {
                        if(client->chats[i]->messages[j]->id == messageid)
                        {
                            printf("Message finded\n");
                            fflush(stdout);

                            int k;
                            for(k = j; client->chats[i]->messages[k + 1] != NULL; k++)
                            {
                                client->chats[i]->messages[k] = client->chats[i]->messages[k + 1];
                            }
                            printf("Message replaced\n");
                            fflush(stdout);

                            client->chats[i]->messages = (struct message_info**) realloc(client->chats[i]->messages, sizeof(struct message_info*)* (k + 1));

                            client->chats[i]->messages[k] = NULL;

                            client->chats[i]->last_message = client->chats[i]->messages[k - 1]->text;
                            client->chats[i]->last_message_time = client->chats[i]->messages[k - 1]->date;
                            break;
                        }
                    }
                }
            }
            
            if(client->callbacks->get_delete_message != NULL)
                client->callbacks->get_delete_message(client);
            break;
        }
        case CREATE_CHAT:
        {
            printf("CREATE CHAT\n");
            fflush(stdout);
            cJSON* chat_json = cJSON_GetObjectItem(cJSON_Parse(resp->json_info), "chat");

            int i;
            for(i = 0; client->chats[i] != NULL; i++){}

            client->chats = (struct chat_info**) realloc(client->chats, sizeof(struct chat_info*) * (i + 2));
            client->chats[i + 1] = NULL;
            client->chats[i] = dup_chat_info();
            set_chat_info_from_json(cJSON_Print(chat_json), client->chats[i]);

            cJSON_Delete(chat_json);

            if(client->callbacks->get_create_chat != NULL)
                client->callbacks->get_create_chat(client);
            break;
        }
        case SEARCH_USERS_BY_TAG_ID:
        {
            cJSON* users_json = cJSON_GetObjectItem(cJSON_Parse(resp->json_info), "users");

            printf("USERS: %s\n", cJSON_Print(users_json));
            int users_size = cJSON_GetArraySize(users_json);
            printf("USERS SIZE: %d\n", users_size);
            fflush(stdout);

            client->searced_users = (struct user_info**) realloc(client->searced_users, sizeof(struct user_info*) * (users_size + 1));
            client->searced_users[users_size] = NULL;

            for (int i = 0; i < users_size; i++)
            {
                client->searced_users[i] = dup_user_info();
                printf("USER %d : %s\n", i, cJSON_Print(cJSON_GetArrayItem(users_json, i)));

                set_user_info_from_json(cJSON_Print(cJSON_GetArrayItem(users_json, i)), client->searced_users[i]);
                
            }

            cJSON_Delete(users_json);

            if(client->callbacks->get_search_users_by_tag_id != NULL)
                client->callbacks->get_search_users_by_tag_id(client);

            for (int i = 0; client->searced_users[i] != NULL; i++)
            {
                free(client->searced_users[i]->image);
                free(client->searced_users[i]->lastname);
                free(client->searced_users[i]->name);
                free(client->searced_users[i]->phone_number);

                for (int j = 0; client->searced_users[i]->tags[j] != NULL; j++)
                {
                    free(client->searced_users[i]->tags[j]->name);
                    free(client->searced_users[i]->tags[j]);
                }
                free(client->searced_users[i]);
            }

            client->searced_users = (struct user_info**) malloc(sizeof(struct user_info*));
            client->searced_users[0] = NULL;
            
            break;
        }
        case ADD_MEMBER_BY_ID:
        {
            int chatid = cJSON_GetObjectItem(cJSON_Parse(resp->json_info), "id_chat")->valueint;

            cJSON* user_json = cJSON_GetObjectItem(cJSON_Parse(resp->json_info), "user");

            for(int i = 0; client->chats[i] != NULL; i++)
            {
                if(client->chats[i]->id == chatid)
                {
                    printf("CHAT FINDED\n");
                    fflush(stdout);
                    int size;
                    for(size = 0; client->chats[i]->users[size] != NULL; size++){}

                    client->chats[i]->users = (struct user_info**) realloc(client->chats[i]->users, sizeof(struct user_info*) * (size + 2));
                    client->chats[i]->users[size + 1] = NULL;
                    client->chats[i]->users[size] = dup_user_info();
                    set_user_info_from_json(cJSON_Print(user_json), client->chats[i]->users[size]);

                    printf("members added\n");
                    fflush(stdout);
                }
            }

            cJSON_Delete(user_json);
            
            if(client->callbacks->get_add_member_by_id != NULL)
                client->callbacks->get_add_member_by_id(client);
            break;
        }
        case DELETE_CHAT:
        {
            if(client->tags[0] != NULL)
            {
                if(client->callbacks->get_delete_chat != NULL)
                    client->callbacks->get_delete_chat(client);
            }
            int chatid = cJSON_GetObjectItem(cJSON_Parse(resp->json_info), "id_chat")->valueint;

            for(int i = 0; client->chats[i] != NULL; i++)
            {
                if(client->chats[i]->id == chatid) //////////////////////////////////
                {
                    printf("Chat finded\n");
                    fflush(stdout);
                    int k;
                    for(k = i; client->chats[k] != NULL; k++)
                    {
                        client->chats[k] = client->chats[k + 1];
                    }
                        printf("Chat replaced\n");
                        fflush(stdout);

                        client->chats[k] = (struct chat_info*) realloc(client->chats, sizeof(struct chat_info) * (k + 1));
                        client->chats[k] = NULL;
                        break;
                }
            }

            if(client->callbacks->get_delete_chat != NULL)
                client->callbacks->get_delete_chat(client);
            break;
        }
        case GET_ALL_TAGS:
        {
            cJSON* tags_json = cJSON_GetObjectItem(cJSON_Parse(resp->json_info), "tags");
            int tags_size = cJSON_GetArraySize(tags_json);

            client->tags = (struct tag_info**) realloc(client->tags, sizeof(struct tag_info*) * (tags_size + 1));
            client->tags[tags_size] = NULL;

            for (int i = 0; i < tags_size; i++)
            {
                client->tags[i] = dup_tag_info();
                set_tag_info_from_json(cJSON_Print(cJSON_GetArrayItem(tags_json, i)), client->tags[i]);
            }

            cJSON_Delete(tags_json);

            if(client->callbacks->get_all_tags != NULL)
                client->callbacks->get_all_tags(client);

            break;
        }
    }
}
