#include <stdio.h>
#include "hashmap.h"
#include <fcntl.h>
#include <unistd.h>
#include <sys/uio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <string.h>

/*
Protocol: list of entries: type (2 bits)
*/

#define EXT_SUCCESS 0
#define EXT_ERR_OPEN 1
#define BUFSIZE 1024

char* filename = NULL;
Hashmap *hashmap = NULL;

META_COMMAND_NOT_FOUND = "Meta command not found\n";
OPEN_FILE_MSG = "Use .open to create a database\n";

int main() {
    char* line;
    while (1) {
        line = readline("> ");
        char** tokens = tokenize(line);
        if (tokens == NULL) {
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
        printf(META_COMMAND_NOT_FOUND);
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
}

void handle_command(char* command, char** tokens) {
    if (filename == NULL) {
        printf(OPEN_FILE_MSG);
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

}