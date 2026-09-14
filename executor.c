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
    OP_MUL
};

int registers[256];

int code_index = -1;

void execute_opcode(uint32_t code) {
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
        break;
    case OP_JUMP:
        if (VERBOSE) {printf("OP_JUMP\n");}
        val = code & 0xFFFFFF;
        if (VERBOSE) {printf("jumping to: %d\n", val);}
        code_index = val;
        break;
    case OP_PNUM:
        if (VERBOSE) {printf("OP_PNUM\n");}
        reg1 = (code >> 16) & 0xFF;
        printf("%d\n", registers[reg1]);
        code_index++;
        break;
    case OP_PCHR:
        if (VERBOSE) {printf("OP_PCHR\n");}
        reg1 = (code >> 16) & 0xFF;
        printf("%c\n", registers[reg1]);
        code_index++;
        break;
    case OP_SWAP:
        if (VERBOSE) {printf("OP_SWAP\n");}
        reg1 = (code >> 16) & 0xFF;
        reg2 = (code >> 8) & 0xFF;
        val = registers[reg1];
        registers[reg1] = registers[reg2];
        registers[reg2] = val;
        if (VERBOSE) {printf("reg1: %d  reg2: %d\n", reg1, reg2);};
        code_index++;
        break;
    case OP_ADD:
        if (VERBOSE) {printf("OP_ADD\n");}
        reg1 = (code >> 16) & 0xFF;
        reg2 = (code >> 8) & 0xFF;
        reg3 = code & 0xFF;
        val = registers[reg1] + registers[reg2];
        registers[reg3] = val;
        if (VERBOSE) {printf("reg1: %d  reg2: %d reg3: %d result: %d\n", reg1, reg2, reg3, val);};
        code_index++;
        break;
    case OP_SUB:
        if (VERBOSE) {printf("OP_SUB\n");}
        reg1 = (code >> 16) & 0xFF;
        reg2 = (code >> 8) & 0xFF;
        reg3 = code & 0xFF;
        val = registers[reg1] - registers[reg2];
        registers[reg3] = val;
        if (VERBOSE) {printf("reg1: %d  reg2: %d reg3: %d result: %d\n", reg1, reg2, reg3, val);};
        code_index++;
        break;
    case OP_MUL:
        if (VERBOSE) {printf("OP_MUL\n");}
        reg1 = (code >> 16) & 0xFF;
        reg2 = (code >> 8) & 0xFF;
        reg3 = code & 0xFF;
        val = registers[reg1] * registers[reg2];
        registers[reg3] = val;
        if (VERBOSE) {printf("reg1: %d  reg2: %d reg3: %d result: %d\n", reg1, reg2, reg3, val);};
        code_index++;
        break;
    default:
        break;
    }
}


int main() {
    uint32_t code[5];
//              aaaaaaaabbbbbbbbccccccccdddddddd
    code[0] = 0b00000000000000010000000000000111;
    code[1] = 0b00000000000000100000000000000110;
    code[2] = 0b00000111000000010000001000000000;
    code[3] = 0b00000010000000000000000000000000;
    while (code_index < 4)
    {
        execute_opcode(code[code_index]);
    }
    
    return 0;
}