#ifndef HASH_H
#define HASH_H

#include <unitypes.h>

typedef uint32_t Hash;

typedef Hash (*HashFunc)(const char* str);
typedef Hash (*BitMixer)(Hash hash);

extern Hash hashf_krlose(const char* str);
extern Hash hashf_djb2(const char* str);
extern Hash hashf_djb2_mix(const char* str, BitMixer mixer);
extern Hash hashf_sdbm(const char* str);
extern Hash hashf_jenkins(const char* str);

extern Hash hash_bitmix32(Hash hash);

#endif
