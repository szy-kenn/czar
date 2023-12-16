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
        printf("Error: Hashmap Collision");
        exit(-4);
    }
    Pair *pair = malloc(sizeof(Pair));
    pair->key = key;
    pair->value = value;
    hashmap->table[idx] = pair;
};

void *hashmap_get(Hashmap *hashmap, char *key) {
    int idx = hash(key);
    return (hashmap->table[idx] != NULL ? hashmap->table[idx]->value : NULL);
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
            free(hashmap->table[i]->key);
            /*
             * this is just a pointer to another state node that will get freed in the loop in
             * another free function in fsmachine
             */
            // free(hashmap->table[i]->value);
            free(hashmap->table[i]);
        }
    }
    free(hashmap);
}
