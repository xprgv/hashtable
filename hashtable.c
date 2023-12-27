#include "hashtable.h"

typedef struct Bucket {
    Hash           hash;
    const void*    key;
    const void*    value;
    struct Bucket* next;
} Bucket;

#define HASH_TABLE_CAPACITY_INIT       32
#define HASH_TABLE_CAPACITY_MULTIPLIER 2
#define HASH_TABLE_GROWTH              1

static size_t _get_bucket_idx(Hash hash, size_t capacity) {
    return hash % capacity;
}

static bool _keys_eq(const void* key1, const void* key2) {
    if (key1 == NULL || key2 == NULL) {
        return false;
    }
    const char* key_str1 = (const char*)key1;
    const char* key_str2 = (const char*)key2;

    return (strcmp(key_str1, key_str2) == 0) ? true : false;
}

static void* _copy(const void* data) {
    if (data == NULL) {
        return NULL;
    }
    const char* input     = (const char*)data;
    size_t      input_len = strlen(input) + 1;
    char*       result    = (char*)malloc(sizeof(*result) * input_len);
    if (result == NULL) {
        return NULL;
    }
    strcpy(result, input);
    return result;
}

extern HashTable* hash_table_new(HashFunc hash_fn) {
    HashTable* ht = (HashTable*)malloc(sizeof(HashTable));
    if (ht == NULL) {
        return NULL;
    }

    ht->size     = 0;
    ht->capacity = HASH_TABLE_CAPACITY_INIT;
    ht->hash_fn  = hash_fn;

    ht->buckets = (Bucket**)malloc(ht->capacity * sizeof(*(ht->buckets)));

    if (ht->buckets == NULL) {
        free(ht);
        return NULL;
    }

    for (int i = 0; i < ht->capacity; ++i) {
        ht->buckets[i] = NULL;
    }

    return ht;
}

extern void hash_table_free(HashTable* ht) {
    Bucket* current = NULL;
    Bucket* next    = NULL;

    for (int i = 0; i < ht->capacity; ++i) {
        current = ht->buckets[i];
        while (current != NULL) {
            next = current->next;
            free(current);
            current = next;
        }
    }

    free(ht->buckets);
    free(ht);
}

extern void hash_table_set(HashTable* ht, const void* key, const void* value) {
    const Hash hash       = ht->hash_fn((const char*)key);
    size_t     bucket_idx = _get_bucket_idx(hash, ht->capacity);
    Bucket*    current    = ht->buckets[bucket_idx];

    if (current == NULL) { // no collision
        Bucket* new_bucket = (Bucket*)malloc(sizeof(Bucket));

        new_bucket->hash  = hash;
        new_bucket->key   = _copy(key);
        new_bucket->value = _copy(value);
        new_bucket->next  = NULL;

        ht->buckets[bucket_idx] = new_bucket;
        ht->size++;
    } else {
        // iterate throw the linked list
        while (current->next != NULL) {
            if (current->hash == hash && _keys_eq(current->key, key)) {
                current->value = value; // update value
                return;
            } else {
                current = current->next;
            }
        }

        Bucket* new_bucket = (Bucket*)malloc(sizeof(Bucket));
        new_bucket->hash   = hash;
        new_bucket->key    = _copy(key);
        new_bucket->value  = _copy(value);
        new_bucket->next   = NULL;

        current->next = new_bucket;
        ht->size++;
    }
}

extern void* hash_table_get(HashTable* ht, const void* key) {
    const Hash hash    = ht->hash_fn((const char*)key);
    Bucket*    current = ht->buckets[_get_bucket_idx(hash, ht->capacity)];

    while (current != NULL) {
        if (current->hash == hash && _keys_eq(current->key, key)) {
            return _copy(current->value);
        }
        current = current->next;
    }

    return NULL; // key does not exists
}

extern bool hash_table_delete(HashTable* ht, const void* key) {
    const Hash hash       = ht->hash_fn((const char*)key);
    size_t     bucket_idx = _get_bucket_idx(hash, ht->capacity);
    Bucket*    current    = ht->buckets[bucket_idx];

    if (current == NULL) {
        return false;
    }

    Bucket* prev = NULL;

    while (current != NULL) {
        if (current->hash == hash && _keys_eq(current->key, key)) {
            current->hash  = 0;
            current->key   = NULL;
            current->value = NULL;

            if (prev == NULL) {              // first node
                if (current->next == NULL) { // no childs
                    ht->buckets[bucket_idx] = NULL;
                } else { // have childs
                    ht->buckets[bucket_idx] = current->next;
                }
            } else {                         // node is not first
                if (current->next == NULL) { // no childs
                    prev->next = NULL;
                } else { // have childs
                    prev->next = current->next;
                }
            }

            free(current);
            current = NULL;
            ht->size--;
            return true;
        }

        prev    = current;
        current = current->next;
    }

    return false;
}

extern bool hash_table_contains(HashTable* ht, const void* key) {
    return (hash_table_get(ht, key) != NULL) ? true : false;
}

extern int hash_table_size(HashTable* ht) {
    return ht->size;
}
