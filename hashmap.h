#include "types.h"

#define KEYSIZE 10
#define TABLESIZE 100

typedef struct node {
    char key[KEYSIZE];
    Schema *value;
} Node;


typedef struct hashmap {
    Node* hm[TABLESIZE];
    int size;
} Hashmap;


Hashmap* hashmap_init();
void hashmap_free(Hashmap *hm);

Schema* hashmap_get(Hashmap *hashmap, char* key);
void hashmap_insert(Hashmap* hashmap, char* key, Schema *value);
void hashmap_print(Hashmap* hashmap);
