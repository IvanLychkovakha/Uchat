#include "../inc/libmx.h"
void mx_printerr_char(char error) {
    write(2, &error, 1);
}
