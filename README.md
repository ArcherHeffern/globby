## Packets
size (bytes)	| field 	| purpose	
-- 				| --		| --
1				| type 		| Set to either 1 or 2 to signify if field is a schema or entry
UNKNOWN			| body		| Body - Either Schema or Entry

## Schema
size (bytes)	| field 	| purpose
-- 				| -- 		| -- 
1				| ssize		| Size of the schema excluding packet type in bytes (max 255)
NULL TERMINATED | sname		| Schema name
1				| dtype		| Type of a field - See Data Types Value Field Below
NULL TERMINATED	| fname		| Field name
1				| end		| Final Null terminator marks the end of a schema. Design Note: Value of this field (NULL TERMINATOR) Can't conflict with dtype options

## Entry
Notes: When parsing, assuming you previously extracted the schema, you should already know:
- The number of fields
- The size of each field
- The size of the entire entry
As such, field_num, null terminator, and entry size fields are omitted

size (bytes)	| field 	| purpose
-- 				| -- 		| -- 
1				| sid		| Number of schema, in order of appearance in the database file. Starting at value 0
See Schema		| value		| Values of entry - See schmea for number and types of fields

## Data Types
Size (bytes)	| Value 	| Type 				| Meaning
-- 				| -- 		| --				| --
1				| 0x00		| NULL TERMINATOR	| Signifies there are no more fields for a schema
1				| 0x01		| CHAR				| Character 
4				| 0x02		| INT				| i32 Integer 
NULL TERMINATED	| 0x03		| STRING			| Null terminated String 

## Grammar
Append only database

<database> = <packet>* NULL_TERMINATOR
<packet> = <schema> | <entry>
<schema> = "0" name_size (1 byte) name num_fields <schema_fields>+
<schema_fields> = field_name_size field_name field_datatype
<entry> = "1" field+

# Lessons
1. Can't write individual bits unless batching multiple bit sized fields together - Then use bitwise operators to get single field
2. Creating better documentation
3. Python bit/byte operators (int.to_bytes, int.from_bytes, bytes, open(file, "rb"), str.encode(), bytes.decode() 
4. Endianness
5. C memory management
6. hexdump uses your computers default architecture - which is apparently little endian. Caused error creating test data since database is big endian
7. Don't coerce small type to large type for arrays - Indexing will skip entries
```c
	// Bad? 
	*arr->array[arr->size++] = schema;
	// Good?
	// arr->array[arr->size++] = &schema;
```
