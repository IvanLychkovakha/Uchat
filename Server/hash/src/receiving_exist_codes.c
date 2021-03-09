#include"../inc/hash.h"

int receiving_exist_codes(int x){
    x += 256;
    while(!(((x <= 57) && (x >=48)) || ((x <= 90) && (x >= 65)) || ((x <= 122) && (x >= 97)))){
        if(x < 48) { x += 24; }
        else {x -= 47; }
    }
    return x;
}
