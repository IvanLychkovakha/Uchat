#include "../inc/database.h"

int mx_check(int rc, char *err)
{
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "SQL error [%s]\n", err);
        sqlite3_free(err);
    }
    return rc;
}
