opcodes = ["OP_LOAD", "OP_JUMP", "OP_PNUM", "OP_PCHR", "OP_SWAP", "OP_ADD", "OP_SUB", "OP_MUL", "OP_END", "OP_IJMP", "OP_OMIT", "OP_PSWP", "OP_GJMP", "OP_NLIN"]

result = []

with open("test.meow", "r") as file:
    for line in file:
        res = ""
        keys = line.strip().split("\n")[0].split(" ")

        if "//" in keys[0] or keys[0] == "":
            opcode = 10
        else:
            opcode = int(opcodes.index(keys[0]))

        instr = 0
        if opcode in [1, 2, 3]: # OP_PNUM, OP_PCHR
            instr = (opcode << 24) | (int(keys[1]) << 16)
        elif opcode == 0:
            instr = (opcode << 24) | (int(keys[1]) << 16) | int(keys[2])
        elif opcode in [4, 11]:
            instr = (opcode << 24) | (int(keys[1]) << 16) | (int(keys[2]) << 8)
        elif opcode in [5, 6, 7, 9, 12]:
            instr = (opcode << 24) | (int(keys[1]) << 16) | (int(keys[2]) << 8) | int(keys[3])
        elif opcode in [8, 10, 13]:
            instr = (opcode << 24)

        result.append(instr.to_bytes(4, byteorder="little"))

with open("test.mbin", "wb") as file:
    for line in result:
        file.write(line)