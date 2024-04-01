#include "types.h"

typedef struct intArray {
    Schema **array;
    int capacity;
    int size;
} IntArray;

IntArray* int_array_init(int capacity);
void int_array_insert(IntArray *arr, Schema *schema);
Schema* int_array_get(IntArray *arr, int index);