# File Format
Type: 1 byte 
Body: Unknown

## Type
0: Schema
1: Entry

## Schema
name_size
Name
fields
(field_name_size
field_name 
datatype)+

# Entry
Payload

Append only database

<database> = <packet>*
<packet> = <schema> | <entry>
<schema> = "0" name_size (1 byte) name num_fields <schema_fields>+
<schema_fields> = field_name_size field_name field_datatype
<entry> = "1" field+
