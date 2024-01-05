#ifndef _HASHMAP_H_
#define _HASHMAP_H_
#define TABLE_SIZE 200

typedef struct {
    char *key;
    void *value;
} Pair;

typedef struct {
    Pair *table[TABLE_SIZE];
} Hashmap;

void hashmap_init(Hashmap *hashmap);
void hashmap_add(Hashmap *hashmap, char *key, void *value);
void hashmap_update(Hashmap *hasmap, char *key, void *new_value);
void *hashmap_get(Hashmap *hashmap, char *key);
void hashmap_delete(Hashmap *hashmap, char *key);
void hashmap_free(Hashmap *hashmap);

#endif