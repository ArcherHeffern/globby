from enum import Enum
from pprint import PrettyPrinter

TESTFILE = "test.bin"

class Type(Enum):
    Bool = 0
    Int = 1
    Str = 2

with open(TESTFILE, "wb+") as f:
	def write_num(num: int, num_bytes: int):
		f.write(int.to_bytes(num, num_bytes, 'big'))

	def write_str(s: str):
		f.write(s.encode())

	def write_null_term_str(s: str):
		write_str(s)
		f.write(int.to_bytes(0, 1, 'big'))
	
	NULL_TERMINATOR = 0x00
	CHAR = 0x01
	INT = 0x02
	STRING = 0x03
	
	def write_schema(sname: str, fields: list[(int, str)]): # type, name
		bsize = 0
		for type_, name in fields:
			bsize += 1
			bsize += len(name) + 1 # 1 including null terminator
		print(bsize)
		write_num(1, 1) # type
		write_num(bsize, 1) # bsize
		write_null_term_str(sname)
		write_num(len(fields), 1) # fnum
		for type_, name in fields:
			write_num(type_, 1)
			write_null_term_str(name)

	
	NULL_TERMINATOR = 0x00
	CHAR = 0x01
	INT = 0x02
	STRING = 0x03
	write_schema("Person", [
		(STRING, "name"),
		(INT, "age"),
		(CHAR, "gender")
	])
	write_schema("Computer", [
		(INT, "Version"),
		(STRING, "Owner"),
		(INT, "Age (years)")
	])
		


with open(TESTFILE, 'rb') as f:
	def read_u1():
		return f.read(1).decode()
	def read_u2():
		return f.read(2).decode()
	def read_n(n: int):
		return f.read(n).decode()

	values = {}
	values["type"] = read_u1()
	values["size"] = read_u1()
	print(values)
