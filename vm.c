#include <stdio.h>
#include <stdint.h>

const int VERBOSE = 0;
const int VERBOSEJP = 0;

enum opcodes {
    OP_LOAD,
    OP_JUMP,
    OP_PNUM,
    OP_PCHR,
    OP_SWAP,
    OP_ADD,
    OP_SUB,
    OP_MUL,
    OP_END,
    OP_IJMP,
    OP_GJMP,
    OP_OMIT,
    OP_PSWP,
    OP_NLIN,
    OP_IADD,
    OP_ISUB,
    OP_IMUL,
    OP_COPY,
    OP_IIJP,
    OP_IGJP,
    OP_UJMP,
    OP_IUJP
};

int registers[256];

int code_index = 0;

int execute_opcode(uint32_t code) {
    int opcode = ((code >> 24) & 0xFF);
    if (opcode == 22) {
        return 1;
    }
    if (opcode == OP_LOAD) {
        if (VERBOSE) {printf("OP_LOAD\n");}
        int r1 = (code >> 16) & 0xFF;
        int v = code & 0xFFFF;
        registers[r1] = v;
        if (VERBOSE) {printf("reg: %d  value: %d actual value: %d\n", r1, v, registers[r1]);};
        code_index++;
        return 0;
    }
    int arg1 = (code >> 16) & 0xFF;
    int arg2 = (code >> 8) & 0xFF;
    int arg3 = code & 0xFF;

    int val;
    switch (opcode)
    {
    case OP_JUMP:
        if (VERBOSE) {printf("OP_JUMP\n");}
        if (VERBOSE) {printf("jumping to: %d\n", arg1);}
        code_index = arg1;
        return 0;
    case OP_PNUM:
        if (VERBOSE) {printf("OP_PNUM\n");}
        printf("%d", registers[arg1]);
        if (VERBOSE) {printf("\n");}
        code_index++;
        return 0;
    case OP_PCHR:
        if (VERBOSE) {printf("OP_PCHR\n");}
        printf("%c", registers[arg1]);
        if (VERBOSE) {printf("\n");}
        code_index++;
        return 0;
    case OP_SWAP:
        if (VERBOSE) {printf("OP_SWAP\n");}
        if (VERBOSE) {printf("BEFORE arg1: %d  arg2: %d\n", arg1, arg2);};
        val = registers[arg1];
        registers[arg1] = registers[arg2];
        registers[arg2] = val;
        if (VERBOSE) {printf("AFTER arg1: %d  arg2: %d\n", arg1, arg2);};
        code_index++;
        return 0;
    case OP_ADD:
        if (VERBOSE) {printf("OP_ADD\n");}
        val = registers[arg1] + registers[arg2];
        registers[arg3] = val;
        if (VERBOSE) {printf("arg1: %d  arg2: %d arg3: %d result: %d\n", arg1, arg2, arg3, val);};
        code_index++;
        return 0;
    case OP_SUB:
        if (VERBOSE) {printf("OP_SUB\n");}
        val = registers[arg1] - registers[arg2];
        registers[arg3] = val;
        if (VERBOSE) {printf("arg1: %d  arg2: %d arg3: %d result: %d\n", arg1, arg2, arg3, val);};
        code_index++;
        return 0;
    case OP_MUL:
        if (VERBOSE) {printf("OP_MUL\n");}
        val = registers[arg1] * registers[arg2];
        registers[arg3] = val;
        if (VERBOSE) {printf("arg1: %d  arg2: %d arg3: %d result: %d\n", arg1, arg2, arg3, val);};
        code_index++;
        return 0;
    case OP_END:
        if (VERBOSE) {printf("OP_END\n");}
        return 1;
    case OP_IJMP:
        if (VERBOSE) {printf("OP_IJMP\n");}
        if (registers[arg1] == registers[arg2]) {
            code_index = arg3;
            if (VERBOSE) {printf("jumping to: %d\n", arg3);}
        } else {
            code_index++;
            if (VERBOSE) {printf("not jumping\n");}
        }
        return 0;
    case OP_GJMP:
        if (VERBOSE) {printf("OP_GJMP\n");}
        if (registers[arg1] > registers[arg2]) {
            code_index = arg3;
            if (VERBOSE) {printf("jumping to: %d\n", arg3);}
        } else {
            code_index++;
            if (VERBOSE) {printf("not jumping");}
        }
        return 0;
    case OP_OMIT:
        if (VERBOSE) {printf("\n");}
        code_index++;
        return 0;
    case OP_PSWP:
        if (VERBOSE) {printf("OP_PSWP\n");}
        int reg1 = registers[arg1];
        if (VERBOSE) {printf("swapping reg1: %d  arg2: %d\n", reg1, arg2);};
        if (VERBOSE) {printf("before values: reg1: %d  reg2: %d\n", registers[reg1], registers[arg2]);};
        val = registers[reg1];
        registers[reg1] = registers[arg2];
        registers[arg2] = val;
        if (VERBOSE) {printf("after values: reg1: %d  reg2: %d\n", registers[reg1], registers[arg2]);};
        code_index++;
        return 0;
    case OP_NLIN:
        if (VERBOSE) {printf("OP_NLIN\n");}
        printf("\n");
        code_index++;
        return 0;
    case OP_IADD:
        if (VERBOSE) {printf("OP_ADD\n");}
        val = registers[arg1] + arg2;
        registers[arg3] = val;
        if (VERBOSE) {printf("arg1: %d  arg2: %d arg3: %d result: %d\n", arg1, arg2, arg3, val);};
        code_index++;
        return 0;
    case OP_ISUB:
        if (VERBOSE) {printf("OP_SUB\n");}
        val = registers[arg1] - arg2;
        registers[arg3] = val;
        if (VERBOSE) {printf("arg1: %d  arg2: %d arg3: %d result: %d\n", arg1, arg2, arg3, val);};
        code_index++;
        return 0;
    case OP_IMUL:
        if (VERBOSE) {printf("OP_MUL\n");}
        val = registers[arg1] * arg2;
        registers[arg3] = val;
        if (VERBOSE) {printf("arg1: %d  arg2: %d arg3: %d result: %d\n", arg1, arg2, arg3, val);};
        code_index++;
        return 0;
    case OP_COPY:
        if (VERBOSE) {printf("OP_COPY\n");}
        registers[arg2] = registers[arg1];
        if (VERBOSE) {printf("COPIED arg1: %d  arg2: %d\n", arg1, arg2);};
        code_index++;
        return 0;
    case OP_IIJP:
        if (VERBOSE) {printf("OP_IIJP\n");}
        if (VERBOSE) {printf("ii arg1: %d val: %d arg2: %d target: %d\n", registers[arg1], val, arg2, arg3);}
        if (registers[arg1] == arg2) {
            code_index = arg3;
            if (VERBOSE) {printf("jumping to: %d\n", arg3);}
        } else {
            code_index++;
            if (VERBOSE) {printf("not jumping\n");}
        }
        return 0;
    case OP_IGJP:
        if (VERBOSE) {printf("OP_IGJP\n");}
        if (registers[arg1] > arg2) {
            code_index = arg3;
            if (VERBOSE) {printf("jumping to: %d\n", arg3);}
        } else {
            code_index++;
            if (VERBOSE) {printf("not jumping");}
        }
        return 0;
    case OP_UJMP:
        if (VERBOSE) {printf("OP_UJMP\n");}
        if (registers[arg1] != registers[arg2]) {
            code_index = arg3;
            if (VERBOSE) {printf("jumping to: %d\n", arg3);}
        } else {
            code_index++;
            if (VERBOSE) {printf("not jumping\n");}
        }
        return 0;
    case OP_IUJP:
        if (VERBOSE) {printf("OP_IUJP\n");}
        if (VERBOSE) {printf("ii arg1: %d val: %d arg2: %d target: %d\n", registers[arg1], val, arg2, arg3);}
        if (registers[arg1] != arg2) {
            code_index = arg3;
            if (VERBOSE) {printf("jumping to: %d\n", arg3);}
        } else {
            code_index++;
            if (VERBOSE) {printf("not jumping\n");}
        }
        return 0;
    default:
        return 0;
    }
}

int main() {
    FILE *file;
    file = fopen("test.mbin", "rb");
    fseek(file, 0, SEEK_END);
    int size = ftell(file);
    fseek(file, 0, SEEK_SET);

    uint32_t code[size / 4];

    fread(code, 4, size / 4, file);
    fclose(file);


    int stop = 0;
    while (!stop)
    {
        stop = execute_opcode(code[code_index]);
    }
    
    return 0;
}