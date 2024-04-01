from enum import Enum
from pprint import PrettyPrinter

TESTFILE = "test.bin"

class Type(Enum):
    Bool = 0
    Int = 1
    Str = 2

with open(TESTFILE, "wb+") as f:
    def write_num(num: int):
        f.write(num.to_bytes())

    def write_str(s: str):
        f.write(s.encode())

    write_num(0) # Schema
    write_num(4) # Name size
    write_str("User")
    write_num(2) # num fields

    write_num(4) # field name size
    write_str("name") # field name
    write_num(Type.Str.value) # field data type

    write_num(2)
    write_str("id")
    write_num(Type.Int.value)

with open(TESTFILE, 'rb') as f:
    def read_u1():
        return f.read(1).decode()
    def read_u2():
        return f.read(2).decode()
    def read_n(n: int):
        return f.read(n).decode()

    values = {}
    values["type"] = ord(read_u1())
    values["name_size"] = ord(read_u1())
    values["name"] = read_n(values["name_size"])
    values["num_fields"] = ord(read_u1())
    values["fields"] = []
    for _ in range(values["num_fields"]):
        field = {}
        field["field_name_size"] = ord(read_u1())
        field["name"] = read_n(field["field_name_size"])
        field["type"] = ord(read_u1())
        values["fields"].append(field)
    PP = PrettyPrinter()
    PP.pprint(values)