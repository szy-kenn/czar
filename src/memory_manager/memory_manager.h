#ifndef _CZAR_MEMORY_MANAGER_H_
#define _CZAR_MEMORY_MANAGER_H_

#define _INIT_BUFFER 8
#include "../utils/utils.h"

void capacity_expand(int capacity);
void memory_expand(size_t size, void *pointer, size_t old_count, size_t new_count);
void memory_free(size_t size, void *pointer, size_t old_count);
void *reallocate(void *pointer, size_t old_size, size_t new_size);

#endif