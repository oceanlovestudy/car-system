#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "package.h"

void* package(char* version, int type, int length, void* data)
{
    struct Header_data* h = malloc(HEADER_SIZE+length);
    strcpy(h->version, version);
    h->type = type;
    h->length = length;
    memcpy(h->data, data, length);

    return (void*)h;
}

