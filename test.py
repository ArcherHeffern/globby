
CHAR = int.to_bytes(0x01)
v = input().encode()
match v:
    case CHAR:
        ...
    case _:
        ...
print("Hello")