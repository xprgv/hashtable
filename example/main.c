#include <stdio.h>

#include "../hashtable.h"

int main(int argc, char* argv[]) {
    HashTable* hash_table = hash_table_new(hashf_djb2);

    hash_table_set(hash_table, "hello", "world");

    printf("hash table size: %d\n", hash_table_size(hash_table));

    printf("%s\n", (char*)hash_table_get(hash_table, "hello"));

    hash_table_delete(hash_table, "hello");
    printf("hash table size: %d\n", hash_table_size(hash_table));

    hash_table_free(hash_table);
    return 0;
}
