#include "array.h"
#include <stdlib.h>
#include "types.h"

IntArray* int_array_init(int capacity) {
    Schema **internal_array = malloc(sizeof(Schema*)*capacity);
    IntArray *arr = malloc(sizeof(IntArray));
    arr->array = internal_array;
    arr->capacity = capacity;
    arr->size = 0;
    return arr;
}

void int_array_insert(IntArray *arr, Schema *schema) {
    if (arr->size == arr->capacity) {
        arr->capacity *= 2;
        arr->array = realloc(arr->array, arr->capacity*sizeof(Schema*));
    }
    arr->array[arr->size++] = schema;
}

Schema* int_array_get(IntArray *arr, int index) {
    if (index >= arr->size) {
        return NULL;
    }
    return arr->array[index];
}