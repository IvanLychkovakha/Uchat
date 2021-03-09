#include "../inc/database.h"

void mx_init_db(sqlite3 **db)
{
    int rc = sqlite3_open(PATH_DB, db);
    if (rc)
    {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(*db));
        exit(EXIT_FAILURE);
    }
    else
    {
        //printf("Opened database successfully\n");

        if (rc != SQLITE_OK)
        {
            fprintf(stderr, "Creation db error: %s\n", sqlite3_errmsg(*db));
            exit(EXIT_FAILURE);
        }
    }
}
