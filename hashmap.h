
#define KEYSIZE 10
#define VALUESIZE 64
#define TABLESIZE 100

typedef struct node {
    char key[KEYSIZE];
    char value[VALUESIZE];
} Node;


typedef struct hashmap {
    Node* hm[TABLESIZE];
    int size;
} Hashmap;


Hashmap* hashmap_init();
void hashmap_free(Hashmap *hm);

char* get(Hashmap *hashmap, char* key);
void insert(Hashmap *hashmap, char* key, char* value);