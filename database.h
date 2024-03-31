
char** tokenize(char* line);
void print_help();
void handle_command(char* command, char** tokens);
int open_database(char* db_name);
void handle_metacommand(char* command, char** tokens);