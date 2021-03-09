#include "../inc/database.h"


char* hash_password(char* password, char *salt)
{
    return strdup(crypt(password, salt));
}