#include"../inc/hash.h"

int get_control_sum(char *str){
    unsigned int sault = 0, strlen = 0;

    for(; strlen < mx_strlen(str); strlen++)
        sault += (int)(str[strlen]);

    return sault;
}
