#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/uio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include "database.h"
#include "hashmap.h"
#include "array.h"

/*
Protocol: list of entries: type (2 bits)
*/

#define EXT_SUCCESS 0
#define EXT_ERR_OPEN 1
#define BUFSIZE 1024
#define INPUTBUFSIZE 64

char* filename = NULL;
Hashmap *name_map = NULL;
IntArray *num_map = NULL;

char *META_COMMAND_NOT_FOUND = "Meta command not found\n";
char *OPEN_FILE_MSG = "Use .open to create a database\n";

int main() {
    char buffer[INPUTBUFSIZE];
    while (1) {
        printf("> ");
        fgets(buffer, INPUTBUFSIZE, stdin);
        char** tokens = tokenize(buffer);
        if (tokens[0] == NULL) {
            continue;
        }
        char* command = tokens[0];
        if (command[0] == '.') {
            handle_metacommand(command, tokens);
        } else {
            handle_command(command, tokens);
        }
    }
}

void handle_metacommand(char* command, char** tokens) {
    if (strcmp(command, ".exit") == 0) {
        exit(EXT_SUCCESS);
    } else if (strcmp(command, ".open") == 0) {
        if (tokens[1] != NULL) {
           open_database(tokens[1]);
        } else {
            printf("Expected database name\n");
        }
    } else if (strcmp(command, ".help") == 0) {
        print_help();
    } else {
        printf("%s", META_COMMAND_NOT_FOUND);
    }
}


int open_database(char* db_name) {
    filename = db_name;
    name_map = hashmap_init();
    num_map = int_array_init(8);
    // Store schemas: schema_name: schema_format + schema bit
    char buffer[BUFSIZE];
    int fd = open(filename, O_RDONLY|O_CREAT);
    if (fd <= 0) {
        perror("Open");
        exit(EXT_ERR_OPEN);
    }

    int n_read;
    int schema_size;
    Schema *schema;
    while (1) {
        if ((n_read = read(fd, buffer, 1)) <= 0) {
            break;
        }
        if (buffer[0] == 0) {
            schema = parse_schema(fd);
            int_array_insert(num_map, schema);
            hashmap_insert(name_map, schema->name, schema);
        } else {
            schema_size = int_array_get(num_map, buffer[0])->size;
            lseek(fd, schema_size, SEEK_CUR);
            // Continue
        }
    }
    // Print Name and num map
    int i;
    for (i = 0; i < num_map->size; i++) {
        schema = int_array_get(num_map, i);
        print_schema(schema);
    }
    return 1;
}

void handle_command(char* command, char** tokens) {
    if (filename == NULL) {
        printf("%s", OPEN_FILE_MSG);
        return;
    }
    if (strcmp(command, "CREATE")) {
        
    }
}

void print_help() {
    printf( "Metacommands: \n"
            ".help                  Display help text\n"
            ".exit                  exit program\n"
            "\n"
            "Commands:\n"
            "CREATE <tablename> <fields>\n"
            "READ <tablename> <id>\n"
            "UPDATE <tablename> <id> <field> <new_value>\n"
            "DELETE <tablename> <id>\n"
    );
}

char** tokenize(char* line) {
    int capacity = 4;
    int size = 1;
    char *del = " \n";
    char **tokens = malloc(sizeof(char*)*capacity);
    char* token;
    tokens[0] = strtok(line, del);
    while ((token = strtok(NULL, del)) != NULL) {
        if (size == capacity) {
            capacity *= 2;
            tokens = realloc(tokens, capacity*sizeof(char*));
        }
        tokens[size++] = token;
    }
    tokens[size] = 0;
    return tokens;
}
