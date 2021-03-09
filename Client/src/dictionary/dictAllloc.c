#include "dictionary.h"

dictionary **dictAlloc(void) 
{
    return malloc(sizeof(dictionary));
}

