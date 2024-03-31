#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define KEYSIZE 16
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

Hashmap* hashmap_init() {
    return malloc(sizeof(Hashmap));
}

void hashmap_free(Hashmap *hm) {
    int i;
    for (i = 0; i < hm->size; i++) {
        free(hm->hm[i]);
    }
    free(hm);
}

Node* node_alloc(char* key, char* value) {
    Node* node = malloc(sizeof(Node));
    strcpy(node->key, key);
    strcpy(node->value, value);
    return node;
}

char* get(Hashmap *hashmap, char* key) {
    int i;
    for (i = 0; i < hashmap->size; i++) {
        if (strcmp(key, hashmap->hm[i]->key) == 0) {
            return hashmap->hm[i]->value;
        }
    }
    return NULL;
}

void insert(Hashmap* hashmap, char* key, char* value) {
    if (hashmap->size >= TABLESIZE) {
        printf("Hashmap full");
    }
    Node* node = node_alloc(key, value);
    hashmap->hm[hashmap->size++] = node;
}