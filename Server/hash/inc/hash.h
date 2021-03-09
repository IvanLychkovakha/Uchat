#ifndef HASH
#define HASH

#include"../../libmx/inc/libmx.h"

#define MAX_HASH_LEN 32

typedef struct s_hash {
    char str[MAX_HASH_LEN];

} t_hash;

int receiving_exist_codes(int x);
int get_control_sum(char *str);
void get_hash(t_hash *hash, char* user_string, unsigned int lenght_hash);

#endif
