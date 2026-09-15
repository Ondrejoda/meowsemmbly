#include <stdio.h>
#include <stdint.h>

const int VERBOSE = 1;

enum opcodes {
    OP_LOAD,
    OP_JUMP,
    OP_PNUM,
    OP_PCHR,
    OP_SWAP,
    OP_ADD,
    OP_SUB,
    OP_MUL,
    OP_END
};

int registers[256];

int code_index = 0;

int execute_opcode(uint32_t code) {
    int opcode = (code >> 24) & 0xFF;
    int reg1;
    int reg2;
    int reg3;
    int val;
    switch (opcode)
    {
    case OP_LOAD:
        if (VERBOSE) {printf("OP_LOAD\n");}
        reg1 = (code >> 16) & 0xFF;
        val = code & 0xFFFF;
        registers[reg1] = val;
        if (VERBOSE) {printf("reg: %d  value: %d\n", reg1, val);};
        code_index++;
        return 0;
    case OP_JUMP:
        if (VERBOSE) {printf("OP_JUMP\n");}
        val = code & 0xFFFFFF;
        if (VERBOSE) {printf("jumping to: %d\n", val);}
        code_index = val;
        return 0;
    case OP_PNUM:
        if (VERBOSE) {printf("OP_PNUM\n");}
        reg1 = (code >> 16) & 0xFF;
        printf("%d\n", registers[reg1]);
        code_index++;
        return 0;
    case OP_PCHR:
        if (VERBOSE) {printf("OP_PCHR\n");}
        reg1 = (code >> 16) & 0xFF;
        printf("%c\n", registers[reg1]);
        code_index++;
        return 0;
    case OP_SWAP:
        if (VERBOSE) {printf("OP_SWAP\n");}
        reg1 = (code >> 16) & 0xFF;
        reg2 = (code >> 8) & 0xFF;
        val = registers[reg1];
        registers[reg1] = registers[reg2];
        registers[reg2] = val;
        if (VERBOSE) {printf("reg1: %d  reg2: %d\n", reg1, reg2);};
        code_index++;
        return 0;
    case OP_ADD:
        if (VERBOSE) {printf("OP_ADD\n");}
        reg1 = (code >> 16) & 0xFF;
        reg2 = (code >> 8) & 0xFF;
        reg3 = code & 0xFF;
        val = registers[reg1] + registers[reg2];
        registers[reg3] = val;
        if (VERBOSE) {printf("reg1: %d  reg2: %d reg3: %d result: %d\n", reg1, reg2, reg3, val);};
        code_index++;
        return 0;
    case OP_SUB:
        if (VERBOSE) {printf("OP_SUB\n");}
        reg1 = (code >> 16) & 0xFF;
        reg2 = (code >> 8) & 0xFF;
        reg3 = code & 0xFF;
        val = registers[reg1] - registers[reg2];
        registers[reg3] = val;
        if (VERBOSE) {printf("reg1: %d  reg2: %d reg3: %d result: %d\n", reg1, reg2, reg3, val);};
        code_index++;
        return 0;
    case OP_MUL:
        if (VERBOSE) {printf("OP_MUL\n");}
        reg1 = (code >> 16) & 0xFF;
        reg2 = (code >> 8) & 0xFF;
        reg3 = code & 0xFF;
        val = registers[reg1] * registers[reg2];
        registers[reg3] = val;
        if (VERBOSE) {printf("reg1: %d  reg2: %d reg3: %d result: %d\n", reg1, reg2, reg3, val);};
        code_index++;
        return 0;
    case OP_END:
        if (VERBOSE) {printf("OP_END\n");}
        return 1;
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