#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/uio.h>
#include "types.h"

enum Type type_mapping[3] = {Bool, Int, Str};

char* type_to_string(enum Type type) {
    switch (type) {
        case Bool:
            return "Bool"; 
        case Int:
            return "Int"; 
        case Str: 
            return "Str";
        default:
            printf("Type %u not found\n", type);
            return NULL;
    }
}

Field* field_init(char* name, enum Type datatype) {
    // Assumes name is already malloced
    Field *field = malloc(sizeof(Field));
    field->datatype = datatype;
    field->name = name;
    return field; 
}

void field_free(Field *field) {
    free(field->name);
    free(field);
}


void print_schema(Schema *schema) {
    printf("%s:%d bytes:%d fields (", schema->name, schema->size, schema->num_fields);
    int i;
    Field *curr_field;
    if (schema->num_fields == 0) {
        printf(")\n");
        return;
    }
    curr_field = schema->fields[0];
    printf("%s: %s", curr_field->name, type_to_string(curr_field->datatype));
    for (i = 1; i < schema->num_fields; i++) {
        curr_field = schema->fields[i];
        printf(", %s: %s", curr_field->name, type_to_string(curr_field->datatype));
    }
    printf(")\n");
}


Schema* parse_schema(int fd) {
    // Assume the schema bit has already been read
    // Assume the file is formatted correctly: Thus no error handling
    int i;
    char buffer[BUFSIZE];
    int name_len;
    char* name;
    int num_fields;
    Field **fields;
    char *field_name;
    enum Type datatype;

    name_len = read_str(fd, buffer, &name);

    read(fd, buffer, 1);
    num_fields = (int)buffer[0];

    fields = malloc(sizeof(Field*) * num_fields);
    for (i = 0; i < num_fields; i++) {
        name_len = read_str(fd, buffer, &field_name);
        printf("%s: Length: %d\n", field_name, name_len);
        read(fd, buffer, 1);
        datatype = type_mapping[buffer[0]];
        fields[i] = field_init(field_name, datatype);
    }
    return schema_init(name, num_fields, fields);
}

Schema* schema_init(char* name, int num_fields, Field **fields) {
    // Assume everything has already been malloced
    int i;
    Schema *schema; 
    int size;

    schema = malloc(sizeof(Schema));
    schema->name = name;
    schema->num_fields = num_fields;
    schema->fields = fields;

    size = 0;
    for (i = 0; i < num_fields; i++) {
        size += fields[i]->datatype;
    }
    schema->size = size;
    return schema;
}

void schema_free(Schema *schema) {
    free(schema->name);
    int i;
    for (i = 0; i < schema->num_fields; i++) {
        field_free(schema->fields[i]);
    }
    free(schema);
}

int read_str(int fd, char* buffer, char** str) {
    int str_size; 
    char* tmp_str;
    read(fd, buffer, 1);
    str_size = buffer[0];
    tmp_str = malloc(str_size + 1);
    read(fd, buffer, str_size);
    buffer[str_size] = 0;
    strcpy(tmp_str, buffer);
    *str = tmp_str;
    return str_size;
}