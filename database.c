#include <stdio.h>
#include <stdint.h>
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

#define SCHEMA 0x1
#define ENTRY 0x2

#define EXT_SUCCESS 0
#define EXT_ERR_OPEN 1
#define BUFSIZE 1024
#define INPUTBUFSIZE 64

char* filename = NULL;

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
	int fd;
    int i;
    int n_read;
    int schema_size;
	int type;
	int size;
    char buffer[BUFSIZE];
    Schema *schema;
	intArray **schema_array = int_array_init(8)

    filename = db_name;
    fd = open(filename, O_RDONLY|O_CREAT);
    if (fd <= 0) {
        perror("Open");
        exit(EXT_ERR_OPEN);
    }
    while (1) {
        if ((n_read = read(fd, buffer, 2)) <= 0) {
            break;
        }
		if (n_read != 2) {
			fprintf(stderr, "Expected 2 bytes in header but found %d\n", n_read);
			exit(1);
		}
		type = buffer[0];
		next = buffer[1];
		switch (type) {
			case ENTRY:
				size = schmea_array[next]->size;
				lseek(fd, size + 1, SEEK_CUR);
				continue;
			case SCHEMA:
				read(fd, buffer, ssize);
				schema = parse_schema(buffer, ssize);
				int_array_insert(schema_array, schema);
				
				break;
			default:
				fprintf(stderr, "Expected 0|1 when parsing entry type but found %\n", type);
				exit(1);
		}
    }
	hashmap_print(name_map);
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
