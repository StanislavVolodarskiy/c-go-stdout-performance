#ifndef BUFFER_T_H_
#define BUFFER_T_H_

#include <stdbool.h>
#include <stdio.h>

typedef struct {
    size_t size;
    size_t pos;
    unsigned char *data;
    FILE *file;
} buffer_t;

bool binit(buffer_t *buffer, size_t size, FILE *file);
void bterm(buffer_t *buffer);
bool bflush(buffer_t *buffer);
bool bwrite(buffer_t *buffer, size_t size, void *data);

#endif
