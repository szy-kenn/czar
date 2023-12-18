#include "memory_manager.h"
#include <stdlib.h>

int capacity_expand(int capacity) {
    return (capacity < _INIT_BUFFER ? _INIT_BUFFER : (capacity) * 2);
}

void *memory_expand(size_t size, void *pointer, size_t old_count,
                    size_t new_count) {
    return reallocate(pointer, size * old_count, size * new_count);
}

void memory_free(size_t size, void *pointer, size_t old_count) {
    reallocate(pointer, size * old_count, 0);
}

void *reallocate(void *pointer, size_t old_size, size_t new_size) {
    if (new_size == 0) {
        free(pointer);
        return NULL;
    }

    void *res = realloc(pointer, new_size);
    if (res == NULL) {
        exit(-3);
    }

    return res;
}