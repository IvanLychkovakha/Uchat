#include "dictionary.h"

void delItem(dictionary **dict, int key) 
{
    dictionary *ptr, *prev;
    for (ptr = *dict, prev = NULL; ptr != NULL; prev = ptr, ptr = ptr->next) 
    {
        if (ptr->key == key) 
        {
            if (ptr->next != NULL) 
            {
                if (prev == NULL) 
                {
                    *dict = ptr->next;
                } 
                else 
                {
                    prev->next = ptr->next;
                }
            } 
            else if (prev != NULL) 
            {
                prev->next = NULL;
            } 
            else 
            {
                *dict = NULL;
            }
            free(ptr);
            return;
        }
    }
}

