database: database.c hashmap.c types.c array.c
	gcc -o database database.c hashmap.c types.c array.c

debug: database.c hashmap.c types.c array.c
	gcc -g -o database database.c hashmap.c types.c array.c