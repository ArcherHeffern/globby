## Packets
General format

size (bytes)	| field 	| purpose	
-- 				| --		| --
1				| type 		| Set to either 1 or 2 to signify if field is a schema or entry
1				| bsize		| Size of body in bytes (max 255)
UNKNOWN			| body		| Body - Either Schema or Entry

## Schema
size (bytes)	| field 	| purpose
-- 				| -- 		| -- 
NULL TERMINATED | sname		| Schema name
4				| fnum		| Number of fields
1				| dtype		| Type of a field - See Data Types Value Field Below
See Dtype		| dentry	| Value of a field - See Data Types Size Field Below
NULL TERMINATED	| fname		| Field name

## Entry
size (bytes)	| field 	| purpose
-- 				| -- 		| -- 
NULL TERMINATED | sname		| Name of schmea this entry belongs to
See Schema		| value		| Values of entry - See schmea for number and types of fields

## Data Types
Size (bytes)	| Value 	| Type 				| Meaning
-- 				| -- 		| --				| --
1				| 0x00		| NULL TERMINATOR	| Signifies there are no more fields for a schema
1				| 0x01		| CHAR				| Character 
4				| 0x10		| INT				| i32 Integer 
NULL TERMINATED	| 0x11		| STRING			| Null terminated String 

## Grammar
Append only database

<database> = <packet>*
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
```c
	// Bad? 
	*arr->array[arr->size++] = schema;
	// Good?
	// arr->array[arr->size++] = &schema;
```
