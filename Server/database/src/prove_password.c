#include "../inc/database.h"

int prove_password(char* password, char* hashed_password)
{
    char* result_hash = strdup(crypt(password, hashed_password));
    printf("RESULT_HASH: %s\n", result_hash);
    return strcmp(result_hash, hashed_password);
}
