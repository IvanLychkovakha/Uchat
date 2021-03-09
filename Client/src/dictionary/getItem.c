#include "dictionary.h"


void *getItem(dictionary *dict, int key) 
{
    dictionary *ptr;
    for (ptr = dict; ptr != NULL; ptr = ptr->next) 
    {
        if (ptr->key == key) 
        {
            return ptr->value;
        }
    }
    return NULL;
}

