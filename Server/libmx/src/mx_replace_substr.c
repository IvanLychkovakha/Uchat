#include "../inc/libmx.h"
//Ururururu ru tassssssssss
//len - n*ru + n*tas
char *mx_replace_substr(const char *str, const char *sub, const char *replace) {
    if (str == NULL || sub == NULL || replace == NULL)
        return NULL;
    int len_str = mx_strlen(str);
    int len_sub = mx_strlen(sub);
    int len_replace = mx_strlen(replace);
    int count_sub = mx_count_substr(str, sub);

    int len_new_str = len_str - count_sub * len_sub + count_sub * len_replace;

    char *new_str = mx_strnew(len_new_str);
    char *ptr_new_str = new_str;

    for(int i = 0; *str; str++) {
        if(*str == sub[i]) 
            i++;
        else {
            i = 0;
            *new_str++ = *str;
        }
        if (i == len_sub) {
            for(int j = 0; j < len_replace; j++) {
                *new_str++ = replace[j];
            }
            i = 0;
        }
    }

    return ptr_new_str;

}
