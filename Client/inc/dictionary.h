#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TEST TRUE /* Comment this line out to compile without a main function (used when including into another application). */

typedef struct dict_t_struct 
{
    int key;
    void *value;
    struct dict_t_struct *next;
} dictionary;

dictionary **dictAlloc(void);
void dictDealloc(dictionary **dict);
void *getItem(dictionary *dict, int key);
void delItem(dictionary **dict, int key);
void addItem(dictionary **dict, int key, void *value);

