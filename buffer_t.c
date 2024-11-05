#include <buffer_t.h>

#include <stdlib.h>
#include <string.h>

bool binit(buffer_t *buffer, size_t size, FILE *file) {
    unsigned char *data = malloc(size);
    if (data == NULL) {
        return false;
    }
    buffer->size = size;
    buffer->pos = 0;
    buffer->data = data;
    buffer->file = file;
    return true;
}

void bterm(buffer_t *buffer) {
    free(buffer->data);
    buffer->file = NULL;
    buffer->data = NULL;
    buffer->pos = 0;
    buffer->size = 0;
}

bool bflush(buffer_t *buffer) {
    size_t written = fwrite(buffer->data, 1, buffer->pos, buffer->file);
    if (written < buffer->pos) {
        return false;
    }
    buffer->pos = 0;
    fflush(buffer->file);
    return true;
}

bool bwrite(buffer_t *buffer, size_t size, void *data) {
    while (size > 0) {
        size_t space = buffer->size - buffer->pos;
        if (size <= space) {
            memcpy(buffer->data + buffer->pos, data, size);
            buffer->pos += size;
            return true;
        }
        if (buffer->pos == 0) {
            return fwrite(data, 1, size, buffer->file) == size;
        }
        memcpy(buffer->data + buffer->pos, data, space);
        buffer->pos += space;
        size -= space;
        data = (unsigned char *)data + space;
        if (!bflush(buffer)) {
            return false;
        }
    }
    return true;
}
