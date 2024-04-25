#include <unistd.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/uio.h>
#include "types.h"

char* type_to_string(Type type) {
    switch (type) {
        case CHAR:
            return "CHAR"; 
        case INT:
            return "INT"; 
        case STRING: 
            return "STRING";
        default:
            printf("Type %u not found\n", type);
            return NULL;
    }
}

Field* field_init(char* name, Type datatype) {
    // Assumes name is already malloced
    Field *field = malloc(sizeof(Field));
    field->dtype = datatype;
    field->fname = name;
    return field; 
}

void field_free(Field *field) {
    free(field->fname);
    free(field);
}


void print_schema(Schema *schema) {
    printf("%s:%d fields (", schema->sname, schema->ssize);
    int i;
    Field *curr_field;
    if (schema->fnum == 0) {
        printf(")\n");
        return;
    }
    curr_field = schema->fields[0];
    printf("%s: %s", curr_field->fname, type_to_string(curr_field->dtype));
    for (i = 1; i < schema->fnum; i++) {
        curr_field = schema->fields[i];
        printf(", %s: %s", curr_field->fname, type_to_string(curr_field->dtype));
    }
    printf(")\n");
}

typedef struct scanner {
	int curr;
	uint8_t* str;
} Scanner;

int consume_next(Scanner *s) {
	return s->str[s->curr++];
}

uint8_t* read_str(Scanner *s) {
	int n = 0;
	while (s->str[s->curr + n] != NULL_TERMINATOR) {
		n++;
	}
	char* o = malloc(n);
	for (int i = 0; i < n; i++) {
		o[i] = s->str[s->curr + i];
	}
	s->curr+= n;
	return o;
}

Schema* parse_schema(uint8_t* buffer, uint8_t ssize) {
    // Assume the file is formatted correctly: Thus no error handling
    int i;
    int name_len;
    uint8_t* name;
    int fnum;
    Field **fields;
    char *field_name;
    Type datatype;

	Scanner s = { 0, buffer };

    name = read_str(&s);
	printf("%s\n", name);
	exit(0);

	datatype = consume_next(&s);
	while (datatype != NULL_TERMINATOR) {
		fnum += 1;
		field_name = read_str(&s);
        fields[i] = field_init(field_name, datatype);
    }
    return schema_init(name, ssize, fnum, fields);
}

Schema* schema_init(char* name, uint8_t ssize, uint8_t fnum, Field **fields) {
    // Assume everything has already been malloced
    int i;
    Schema *schema; 
    int size;

    schema = malloc(sizeof(Schema));
	schema->ssize = ssize;
    schema->sname = name;
	schema->fnum = fnum;
    schema->fields = fields;

    return schema;
}

void schema_free(Schema *schema) {
    free(schema->sname);
    int i;
    for (i = 0; i < schema->fnum; i++) {
        field_free(schema->fields[i]);
    }
    free(schema);
}

int main() {
	// Size and Packet type are omitted
	uint8_t test[] = {0x50, 0x65, 0x72, 0x73, 0x6f, 0x6e, 0x00, 0x03, 0x6e, 0x61, 0x6d, 0x65, 0x00, 0x02,0x61, 0x67, 0x65, 0x00, 0x01, 0x67, 0x65, 0x6e, 0x64, 0x65, 0x72, 0x00, 0x00, 0x00};
	Schema *schema = parse_schema(test, 30);
	print_schema(schema);
}
