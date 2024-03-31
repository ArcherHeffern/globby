#define BUFSIZE 1024

#ifndef _MY_TYPES_
#define _MY_TYPES_

enum Type {
    Bool = 1,
    Int = 4,
    Str = 20, 
};

typedef struct field {
    char* name;
    enum Type datatype; 
} Field;

typedef struct schema {
    char* name;
    int size; // For skipping 
    int num_fields;
    Field **fields;
} Schema;


void print_schema(Schema *schema);
Schema* schema_init(char* name, int num_fields, Field **fields);
void schema_free(Schema *schema);
Field* field_init(char* name, enum Type datatype);
void field_free(Field *field);
Schema* parse_schema(int fd);
int read_str(int fd, char* buffer, char** str);

#endif