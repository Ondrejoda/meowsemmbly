opcodes = ["LOAD", "JUMP", "PNUM", "PCHR", "SWAP", "ADD", "SUB", "MUL", "END", "IJMP", "_omit", "PSWP", "GJMP", "NLIN", ":"]
#           0       1       2       3       4       5      6      7      8      9       10       11      12      13      14

result = []

point_names = []

with open("test.meow", "r") as file:
    linecount = 0

    lines = []

    for line in file:
        lines.append(line)

    for line in lines:
        keys = line.strip().split("\n")[0].split(" ")
        if "//" in keys[0] or keys[0] == "":
            opcode = 10 # _omit
        else:
            opcode = int(opcodes.index(keys[0]))
        if opcode == 14: # _jpnt
            point = keys[1]
            if not point in point_names:
                point_names.append(point)

    linecount = 0

    for line in lines:
        keys = line.strip().split("\n")[0].split(" ")

        if "//" in keys[0] or keys[0] == "":
            opcode = 10 # _omit
        else:
            opcode = int(opcodes.index(keys[0]))

        if opcode in [1, 9, 12]: # JUMP, IJMP, GJMP
            for i in range(len(keys)):
                key = keys[i]
                if key in point_names:
                    keys[i] = point_names.index(key)

        instr = 0
        if opcode in [1, 2, 3]: # JUMP, PNUM, PCHR
            instr = (opcode << 24) | (int(keys[1]) << 16)
        elif opcode == 0: # LOAD
            instr = (opcode << 24) | (int(keys[1]) << 16) | int(keys[2])
        elif opcode == 14: # _jpnt
            point_num = point_names.index(keys[1])
            instr = (opcode << 24) | (int(point_num) << 16) | linecount
        elif opcode in [4, 11]: # SWAP, PSWP
            instr = (opcode << 24) | (int(keys[1]) << 16) | (int(keys[2]) << 8)
        elif opcode in [5, 6, 7, 9, 12]: # ADD, SUB, MUL, IJMP, GJMP
            instr = (opcode << 24) | (int(keys[1]) << 16) | (int(keys[2]) << 8) | int(keys[3])
        elif opcode in [8, 10, 13]: # END, _omit, _jpnt
            instr = (opcode << 24)

        result.append(instr.to_bytes(4, byteorder="little"))
        linecount += 1

with open("test.mbin", "wb") as file:
    for line in result:
        file.write(line)