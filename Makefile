
.DEFAULT_GOAL = build

build:
	cc -c -Wall -Werror -fpic hashtable.h hashtable.c
	cc -shared -o libhashtable.so hashtable.o

clean:
	rm hashtable.h.gch hashtable.o libhashtable.so

example: 
	cc -o ht-example example/main.c hashtable.h hashtable.c hash/hash.h hash/hash.c
	./ht-example
	rm ht-example
