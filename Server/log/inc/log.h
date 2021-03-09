#ifndef LOG
#define LOG

#include"../../libmx/inc/libmx.h"

/*
    [level log] [system info] [message] [context]\n

    level log: уровень логирования в зависимости от ситуации.

        INFO - информационное сообщение о работе программы.

        WARNING - это или информационное сообщение, или ошибка.

        FATAL - фактически заносит сообщение в лог, а затем вызывает exit(1)
            В принципе это означает:
            1) отложенные выражения в других подпрограммах(горутинах) не выполняются;
            2) буферы не очищаются;
            3) временные файлы и каталоги не удаляются.

    system info: метка времени, ид процесса, ид потока и другая служебная информация

    message: текст сообщения

    context: любая дополнительная информация, контекст может быть общим для сообщений в рамках какой то операции.
*/

#define BUFSIZ_SYSINFO 64
#define BUFSIZ_STR 256     

#define INFO    "INFO"
#define WARNING "WARNING"
#define FATAL   "FATAL" 

typedef struct s_log {
    FILE *fd;
    const char *logging_lvl;
    char system_info[BUFSIZ_SYSINFO];
    char message[BUFSIZ_STR];
} t_log;

int create_log(t_log *log, char* filename, char *filemode);
void logging(t_log *log, const char *logging_level, const char *system_info, char *format, ...);

#endif
