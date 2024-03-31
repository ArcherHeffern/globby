#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/uio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include "database.h"
#include "hashmap.h"

/*
Protocol: list of entries: type (2 bits)
*/

#define EXT_SUCCESS 0
#define EXT_ERR_OPEN 1
#define BUFSIZE 1024
#define INPUTBUFSIZE 64

char* filename = NULL;
Hashmap *hashmap = NULL;

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
    hashmap = hashmap_init();
    // Store schemas: schema_name: schema_format + schema bit
    char buffer[BUFSIZE];
    int fd = open(filename, O_RDONLY|O_CREAT);
    if (fd <= 0) {
        perror("Open");
        exit(EXT_ERR_OPEN);
    }

    int n_read;
    while (1) {
        if ((n_read = read(fd, buffer, 1)) <= 0) {
            break;
        }
        if (buffer[0] == '0') {
            // Schema
        } else {

        }
    }
    // TODO
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
    printf( "Metacommands: "
            ".help                  Display help text"
            ".exit                  exit program"
            ""
            "Commands:"
            "CREATE <tablename> <fields>"
            "READ <tablename> <id>"
            "UPDATE <tablename> <id> <field> <new_value>"
            "DELETE <tablename> <id>"
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
            tokens = realloc(tokens, capacity*2*sizeof(char*));
        }
        tokens[size++] = token;
    }
    tokens[size] = 0;
    return tokens;
}