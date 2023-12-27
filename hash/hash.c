#include "hash.h"

static Hash _hash_bitmix32(Hash hash) {
    hash ^= hash >> 16;
    hash *= 0x3243f6a9U;
    hash ^= hash >> 16;
    return hash;
}

extern Hash hash_bitmix32(Hash hash) {
    return _hash_bitmix32(hash);
}

extern Hash hashf_division(Hash x, Hash m) {
    return x % m;
}

#define hash_a ((Hash)2654435769)
#define hash_w 32
#define hash_m 3

extern Hash hashf_multip(Hash x, Hash m) {
    return (x * hash_a) >> (hash_w - m);
}

extern Hash hashf_multip2(Hash x, Hash m) {
    x ^= x >> (hash_w - m);
    return (x * hash_a) >> (hash_w - m);
}

// just sum all uint8 values of an ascii string
extern Hash hashf_krlose(const char* str) {
    Hash hash = 0;
    while (*str) {
        hash += *str;
        str++;
    }
    return hash;
}

#define HASH_DJB2_SEED       5381
#define HASH_DJB2_MULTIPLIER 33

// Bernstein hash djb2
static Hash _hashf_djb2(const char* str) {
    Hash hash = HASH_DJB2_SEED;
    while (*str) {                          // while char is not '\n'
        hash = ((hash << 5) + hash) + *str; // hash * 33 + c
        str++;
    }
    return hash;
}

extern Hash hashf_djb2(const char* str) {
    return _hash_bitmix32(_hashf_djb2(str));
}

extern Hash hashf_djb2_fin(const char* str, BitMixer mixer) {
    Hash hash = _hashf_djb2(str);
    if (mixer != NULL) {
        hash = mixer(hash);
    }
    return hash;
}

extern Hash hashf_sdbm(const char* str) {
    Hash hash = 0;
    while (*str) {
        hash = *str + (hash << 6) + (hash << 16) - hash;
        str++;
    }
    return hash;
}

extern Hash hashf_jenkins(const char* str) {
    Hash hash = 0;
    while (*str) {
        hash += *str;
        hash += (hash << 10);
        hash ^= (hash >> 6);
        str++;
    }
    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);
    return hash;
}

#define FNV_OFFSET_BASIS 2166136261
#define FNV_PRIME        16777619 // 2**24 + 2**8 + 0x93

Hash hashf_fnv1(char* str) {
    Hash hash = FNV_OFFSET_BASIS;
    while (*str) {
        hash = hash ^ *str;
        hash = hash * FNV_PRIME;
        str++;
    }
    return hash;
}
