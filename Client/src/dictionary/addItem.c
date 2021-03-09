#include "dictionary.h"

void addItem(dictionary **dict, int key, void *value) 
{
    delItem(dict, key); /* If we already have a item with this key, delete it. */
    dictionary *d = malloc(sizeof(dictionary));
    d->key = key;
    d->value = value;
    d->next = *dict;
    *dict = d;
}

