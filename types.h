#ifndef _MY_TYPES_
#define _MY_TYPES_

#include <stdint.h>
#define BUFSIZE 1024

typedef enum { 
	NULL_TERMINATOR = 0x00,
	CHAR = 0x01,
	INT = 0x02,
	STRING = 0x03,
} Type;

typedef struct field {
    uint8_t dtype; // Use DataType Enum here
    char* fname;
} Field;

typedef struct schema {
    uint8_t ssize; // For skipping 
    char* sname;
	uint8_t fnum; // Not in database - To be computed
    Field **fields;
} Schema;

void print_schema(Schema *schema);
Schema* schema_init(char* name, uint8_t ssize, uint8_t fnum, Field **fields);
void schema_free(Schema *schema);
Field* field_init(char* name, Type datatype);
void field_free(Field *field);
Schema* parse_schema(uint8_t* buffer, uint8_t ssize);

#endif
