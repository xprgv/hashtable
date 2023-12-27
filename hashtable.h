#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unitypes.h>

#include "hash/hash.h"

typedef struct HashTable {
    size_t          capacity;
    size_t          size;
    struct Bucket** buckets;
    HashFunc        hash_fn;
} HashTable;

extern HashTable* hash_table_new(HashFunc hf);
extern void       hash_table_free(HashTable* ht);
extern void*      hash_table_get(HashTable* ht, const void* key);
extern bool       hash_table_contains(HashTable* ht, const void* key);
extern void hash_table_set(HashTable* ht, const void* key, const void* value);
extern bool hash_table_delete(HashTable* ht, const void* key);
extern int  hash_table_size(HashTable* ht);

#endif
