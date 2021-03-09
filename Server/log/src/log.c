#include"../inc/log.h"

static void clear_log(t_log *log) {
    log->logging_lvl = NULL;
    bzero(log->system_info, BUFSIZ_SYSINFO);
    bzero(log->message, BUFSIZ_STR);
}

static void lprintf(t_log *log) {
    const char *arr[4];
    arr[0] = log->logging_lvl;
    arr[1] = log->system_info;
    arr[2] = log->message;
    arr[3] = NULL;

    for(int i = 0; arr[i]; i++) {

        fprintf(log->fd, "[%s]", arr[i]);

        if(arr[i+1]) {
            fprintf(log->fd, " ");
        }
    }
    fprintf(log->fd, "\n");
    fflush(log->fd);
    clear_log(log);

}

static void set_logging_level(t_log *log, const char *logging_level) {
    log->logging_lvl = logging_level;
}

static void set_message(t_log *log, char *format, ...) {
    int integer;
    char *integer_to_str;
    char *str;
    int index = 0;

    va_list factor;
    va_start(factor, format);

    for(char *ptr = format; *ptr; ptr++) {

        if(*ptr != '%') {
            log->message[index++] = *ptr;
            continue;
        }

        switch(*++ptr) {
            case 'd':
                integer = va_arg(factor, int);
                integer_to_str = mx_itoa(integer);

                strncpy(&log->message[index], integer_to_str, strlen(integer_to_str));

                index+=strlen(integer_to_str);
                free(integer_to_str);
                break;

            case 's':
                str = va_arg(factor, char*);
                strncpy(&log->message[index], str, strlen(str));

                index+=strlen(str);
                break;
            
        }
    }
    va_end(factor);
}

static void set_system_info(t_log *log, const char *system_info) {
    time_t t;
    int s_ptr;
    char *ct;

    time(&t);
    ct = ctime(&t);
    s_ptr = strlen(ct) - 1;

    strncpy(log->system_info, ct, s_ptr);

    if(system_info) {
        log->system_info[++s_ptr] = ' ';
        strncpy(log->system_info, system_info, BUFSIZ_SYSINFO - s_ptr);
    }
}

int create_log(t_log *log, char* filename, char *filemode) {
    log->fd = fopen(filename, filemode);
    clear_log(log);

    return (int)log->fd;
}

void logging(t_log *log, const char *logging_level, const char *system_info, char *format, ...) {
    int integer;
    char *integer_to_str;
    char *str;
    int index = 0;
    va_list factor;
    va_start(factor, format);

    for(char *ptr = format; *ptr; ptr++) {

        if(*ptr != '%') {
            log->message[index++] = *ptr;
            continue;
        }

        switch(*++ptr) {
            case 'd':
                integer = va_arg(factor, int);
                integer_to_str = mx_itoa(integer);

                strncpy(&log->message[index], integer_to_str, strlen(integer_to_str));

                index+=strlen(integer_to_str);
                free(integer_to_str);
                break;

            case 's':
                str = va_arg(factor, char*);
                strncpy(&log->message[index], str, strlen(str));

                index+=strlen(str);
                break;
            
        }
    }
    va_end(factor);

    set_logging_level(log, logging_level);
    set_system_info(log, system_info);
    lprintf(log);
}
