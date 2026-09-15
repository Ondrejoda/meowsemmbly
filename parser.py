opcodes = ["OP_LOAD", "OP_JUMP", "OP_PNUM", "OP_PCHR", "OP_SWAP", "OP_ADD", "OP_SUB", "OP_MUL", "OP_END"]

result = []

with open("test.meow", "r") as file:
    for line in file:
        res = ""
        keys = line.split("\n")[0].split(" ")
        opcode = int(opcodes.index(keys[0]))

        instr = 0

        if opcode == 1: # OP_JUMP
            instr = (opcode << 24) | (int(keys[1]) & 0xFFFFFF)
        elif opcode in [2, 3]: # OP_PNUM, OP_PCHR
            instr = (opcode << 24) | (int(keys[1]) << 16)
        elif opcode in [0, 4]:
            instr = (opcode << 24) | (int(keys[1]) << 16) | int(keys[2])
        elif opcode in [5, 6, 7]:
            instr = (opcode << 24) | (int(keys[1]) << 16) | (int(keys[2]) << 8) | int(keys[3])
        elif opcode == 8:
            instr = (opcode << 24)

        result.append(instr.to_bytes(4, byteorder="little"))

with open("test.mbin", "wb") as file:
    for line in result:
        file.write(line)