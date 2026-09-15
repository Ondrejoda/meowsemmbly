opcodes = ["LOAD", "JUMP", "PNUM", "PCHR", "SWAP", "ADD", "SUB", "MUL", "END", "IJMP", "GJMP", "_omit", "PSWP", "NLIN", ":", "_iadd", "_isub", "_imul", "COPY", "_iijp", "_igjp"]
#           0       1       2       3       4       5      6      7      8      9       10       11      12      13      14   15       16       17       18      19       20 

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
            opcode = 11 # _omit
        else:
            opcode = int(opcodes.index(keys[0]))
        if opcode == 14: # _jpnt    
            point = keys[1]
            if not point in point_names:
                point_names.append(point)

    linecount = 0

    for line in lines:
        prnt = False
        has_immediate = False
        for char in line:
            if char == ".":
                has_immediate = True
        keys = line.strip().split("\n")[0].split(" ")

        if "//" in keys[0] or keys[0] == "":
            opcode = 11 # _omit
        else:
            opcode = int(opcodes.index(keys[0]))    
            if has_immediate:
                opcode += 10
                keys[0] = opcodes[opcode]
            if opcode in [5, 15, 19]:
                prnt = True

        if opcode in [1, 9, 10, 19, 20]: # JUMP, IJMP, GJMP, _iijp, _igmp
            for i in range(len(keys)):
                key = keys[i]
                if key in point_names:
                    keys[i] = point_names.index(key)


        for i in range(len(keys)):
            key = keys[i]
            if "." in str(key):
                keys[i] = key.split(".")[1]

        instr = 0
        if opcode in [1, 2, 3]: # JUMP, PNUM, PCHR
            instr = (opcode << 24) | (int(keys[1]) << 16)
        elif opcode == 0: # LOAD
            instr = (opcode << 24) | (int(keys[1]) << 16) | int(keys[2])
        elif opcode == 14: # _jpnt
            point_num = point_names.index(keys[1])
            instr = (opcode << 24) | (int(point_num) << 16) | linecount
        elif opcode in [4, 12, 18]: # SWAP, PSWP, COPY
            instr = (opcode << 24) | (int(keys[1]) << 16) | (int(keys[2]) << 8)
        elif opcode in [5, 6, 7, 9, 10, 15, 16, 17, 19, 20]: # ADD, SUB, MUL, IJMP, GJMP, _iadd, _isub, _imul, _iijp, _igjp
            instr = (opcode << 24) | (int(keys[1]) << 16) | (int(keys[2]) << 8) | int(keys[3])
        elif opcode in [8, 11, 13]: # END, _omit, _jpnt
            instr = (opcode << 24)

        result.append(instr.to_bytes(4, byteorder="little"))
        linecount += 1

with open("test.mbin", "wb") as file:
    for line in result:
        file.write(line)