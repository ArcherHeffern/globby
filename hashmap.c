#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "types.h"
#include "hashmap.h"

#ifndef _HASHMAP_
#define _HASHMAP_

// int main() {
//     Hashmap *hm = hashmap_init();
//     Schema *schema;
//     Field *fields[2];
//     fields[0] = field_init("name", Str);
//     fields[1] = field_init("age", Int);
//     schema = schema_init("Archer", 2, fields);
//     print_schema(schema);
// }

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

Node* node_alloc(char* key, Schema *value) {
    Node* node = malloc(sizeof(Node));
    strcpy(node->key, key);
    node->value = value;
    return node;
}

void node_free(Node *node) {
    schema_free(node->value);
    free(node);
}

Schema* get(Hashmap *hashmap, char* key) {
    int i;
    for (i = 0; i < hashmap->size; i++) {
        if (strcmp(key, hashmap->hm[i]->key) == 0) {
            return hashmap->hm[i]->value;
        }
    }
    return NULL;
}

void insert(Hashmap* hashmap, char* key, Schema *value) {
    if (hashmap->size >= TABLESIZE) {
        printf("Hashmap full");
    }
    Node* node = node_alloc(key, value);
    hashmap->hm[hashmap->size++] = node;
}

#endif