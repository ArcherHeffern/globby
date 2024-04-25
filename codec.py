from io import BytesIO

TESTFILE = "test.bin"

# Packet Types
SCHEMA = int.to_bytes(0x01)
ENTRY = int.to_bytes(0x02)

# Schema Data Types
NULL_TERMINATOR = int.to_bytes(0x00)
CHAR = int.to_bytes(0x01)
INT = int.to_bytes(0x02)
STRING = int.to_bytes(0x03)


with open(TESTFILE, "wb+") as f:
	def write_bytes(b: bytes):
		f.write(b)

	def write_num(num: int, num_bytes: int):
		f.write(int.to_bytes(num, num_bytes, 'big'))

	def write_str(s: str):
		f.write(s.encode())

	def write_null_term_str(s: str):
		write_str(s)
		f.write(NULL_TERMINATOR)
	
	def write_schema(sname: str, fields: list[(int, str)]): # type, name
		bsize = 0
		for type_, name in fields:
			bsize += 1
			bsize += len(name) + 1 # 1 including null terminator
		bsize += len(sname) + 1 # 1 including null terminator
		write_bytes(SCHEMA) # type
		write_num(bsize, 1) # bsize
		write_null_term_str(sname)
		for type_, name in fields:
			write_bytes(type_)
			write_null_term_str(name)
		write_bytes(NULL_TERMINATOR)

	def write_entry(sid: int, values: list):
		write_bytes(ENTRY)
		write_num(sid, 1)
		for value in values:
			if type(value) is int:
				write_num(value, 4)
			elif type(value) is not str:
				print(f"Expected string or integer but found {type(value)}")
				exit(1)
			elif len(value) == 1:
				write_str(value)
			else:
				write_null_term_str(value)


	
	write_schema("Person", [
		(STRING, "name"),
		(INT, "age"),
		(CHAR, "gender")
	])
	write_bytes(NULL_TERMINATOR)
		

with open(TESTFILE, 'rb') as f:
	def read_u1() -> bytes:
		return f.read(1)
	def to_int(b: bytes) -> int:
		return int.from_bytes(b)
	def read_i32():
		return int.from_bytes(f.read(4), 'big')
	def read_string():
		b = BytesIO()
		while (v := read_u1()) != NULL_TERMINATOR:
			b.write(v)
		return b.getvalue().decode()

	def read_schema():
		"""Assume type has already been read"""
		values = {}
		values["size"] = to_int(read_u1())
		values["name"] = read_string()
		values["fields"] = []
		while (dtype := read_u1()) != NULL_TERMINATOR:
			if dtype == CHAR:
					name = "CHAR"
			elif dtype == INT:
					name = "INT"
			elif dtype == STRING:
					name = "STRING"
			else:
				print(f"Unexpected value {dtype}")
				exit(1)
			values["fields"].append((read_string(), name)) 
		return values

	def read_entry(schemas):
		values = {}
		values["sid"] = int.from_bytes(read_u1())
		values["fields"] = []

		schema_fields = schemas[values["sid"]]["fields"]
		field_num = 0
		while field_num < len(schema_fields):
			field_type = schema_fields[field_num][1]
			match field_type:
				case "CHAR":
					field = read_u1().decode()
				case "INT":
					field = read_i32()
				case "STRING":
					field = read_string()
				case _:
					print(f"Expected field to be CHAR|INT|STRING but found {field_type}")
					exit(1)
			values["fields"].append(field)
			field_num += 1
		return values

	schemas = []
	entries = []
	while (packet_type := read_u1()) != NULL_TERMINATOR:
		if packet_type == SCHEMA:
			schemas.append(read_schema())
		elif packet_type == ENTRY:
			entries.append(read_entry(schemas))
		else:
			print(f"Expected Packet type but found {packet_type}")
			exit(1)
	print(f"Schemas: {schemas}")
	print(f"Fields: {entries}")
"""
	write_schema("Computer", [
		(INT, "Version"),
		(STRING, "Owner"),
		(INT, "Age (years)")
	])
	write_entry(0, ["Tim", 18, 'M'])
	write_entry(0, ["Jon", 24, 'M'])
	write_entry(0, ["Jenny", 34, 'F'])

"""
