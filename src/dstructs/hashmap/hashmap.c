#include "hashmap.h"
#include <stdio.h>
#include <stdlib.h>

unsigned long hash(unsigned char *str) {
    unsigned long hash = 5381;
    int c;

    while (c = *str++)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash % TABLE_SIZE;
}
void hashmap_init(Hashmap *hashmap) {
    for (int i = 0; i < TABLE_SIZE; i++) {
        hashmap->table[i] = NULL;
    }
}

void hashmap_add(Hashmap *hashmap, char *key, void *value) {
    int idx = hash(key);
    if (hashmap->table[idx] != NULL) {
        printf("COLLISION!!!");
        return;
    }
    Pair *pair = malloc(sizeof(Pair));
    pair->key = key;
    pair->value = value;
    hashmap->table[idx] = pair;
};

Pair *hashmap_get(Hashmap *hashmap, char *key) {
    int idx = hash(key);
    return hashmap->table[idx]->value;
}

void hashmap_update(Hashmap *hashmap, char *key, void *new_value) {
    int idx = hash(key);
    hashmap->table[idx]->value = new_value;
}

void hashmap_delete(Hashmap *hashmap, char *key) {
    int idx = hash(key);
    if (hashmap->table[idx] == NULL) {
        printf("No item deleted");
        return;
    }

    hashmap->table[idx] == NULL;
}

void hashmap_free(Hashmap *hashmap) {
    for (int i = 0; i < TABLE_SIZE; i++) {
        if (hashmap->table[i] != NULL) {
            free(hashmap->table[i]);
        }
    }
    free(hashmap);
}
