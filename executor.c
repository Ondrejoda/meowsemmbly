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
    OP_JPNT,
    OP_IADD,
    OP_ISUB,
    OP_IMUL,
    OP_COPY,
    OP_IIJP,
    OP_IGJP
};

int registers[256];
int jump_points[256];

int code_index = 0;

void locate_jump_points(uint32_t codes[], int size) {
    for (size_t i = 0; i < size; i++) {
        uint32_t code = codes[i];
        int opcode = (code >> 24) & 0xFF;
        int reg1;
        int val;
        if (opcode == OP_JPNT) {
            if (VERBOSEJP) {printf("OP_JPNT\n");}
            reg1 = (code >> 16) & 0xFF;
            val = code & 0xFFFF;
            jump_points[reg1] = val;
            if (VERBOSEJP) {printf("jpnt: %d  value: %d actual value: %d\n", reg1, val, jump_points[reg1]);};
        }
    }

}

int execute_opcode(uint32_t code) {
    int opcode = (code >> 24) & 0xFF;
    int reg1;
    int reg2;
    int reg3;
    int val;
    int target;
    switch (opcode)
    {
    case OP_LOAD:
        if (VERBOSE) {printf("OP_LOAD\n");}
        reg1 = (code >> 16) & 0xFF;
        val = code & 0xFFFF;
        registers[reg1] = val;
        if (VERBOSE) {printf("reg: %d  value: %d actual value: %d\n", reg1, val, registers[reg1]);};
        code_index++;
        return 0;
    case OP_JUMP:
        if (VERBOSE) {printf("OP_JUMP\n");}
        reg1 = (code >> 16) & 0xFF;
        target = jump_points[reg1];
        if (VERBOSE) {printf("jumping to: %d\n", target);}
        code_index = target;
        return 0;
    case OP_PNUM:
        if (VERBOSE) {printf("OP_PNUM\n");}
        reg1 = (code >> 16) & 0xFF;
        printf("%d", registers[reg1]);
        if (VERBOSE) {printf("\n");}
        code_index++;
        return 0;
    case OP_PCHR:
        if (VERBOSE) {printf("OP_PCHR\n");}
        reg1 = (code >> 16) & 0xFF;
        if (VERBOSE) {printf("printing reg: %d with value: %d\n", reg1, registers[reg1]);}
        printf("%c", registers[reg1]);
        if (VERBOSE) {printf("\n");}
        code_index++;
        return 0;
    case OP_SWAP:
        if (VERBOSE) {printf("OP_SWAP\n");}
        reg1 = (code >> 16) & 0xFF;
        reg2 = (code >> 8) & 0xFF;
        if (VERBOSE) {printf("BEFORE reg1: %d  reg2: %d\n", reg1, reg2);};
        val = registers[reg1];
        registers[reg1] = registers[reg2];
        registers[reg2] = val;
        if (VERBOSE) {printf("AFTER reg1: %d  reg2: %d\n", reg1, reg2);};
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
    case OP_IJMP:
        if (VERBOSE) {printf("OP_IJMP\n");}
        reg1 = (code >> 16) & 0xFF;
        reg2 = (code >> 8) & 0xFF;
        reg3 = code & 0xFF;
        target = jump_points[reg3];
        if (registers[reg1] == registers[reg2]) {
            code_index = target;
            if (VERBOSE) {printf("jumping to: %d\n", target);}
        } else {
            code_index++;
            if (VERBOSE) {printf("not jumping\n");}
        }
        return 0;
    case OP_GJMP:
        if (VERBOSE) {printf("OP_GJMP\n");}
        reg1 = (code >> 16) & 0xFF;
        reg2 = (code >> 8) & 0xFF;
        reg3 = code & 0xFF;
        target = jump_points[reg3];
        if (registers[reg1] > registers[reg2]) {
            code_index = target;
            if (VERBOSE) {printf("jumping to: %d\n", target);}
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
        if (VERBOSE) {printf("OP_SWAP\n");}
        int reg1_p = (code >> 16) & 0xFF;
        int reg2_p = (code >> 8) & 0xFF;
        reg1 = registers[reg1_p];
        reg2 = registers[reg2_p];
        if (VERBOSE) {printf("swapping reg1: %d  reg2: %d\n", reg1, reg2);};
        if (VERBOSE) {printf("before values: reg1: %d  reg2: %d\n", registers[reg1], registers[reg2]);};
        val = registers[reg1];
        registers[reg1] = registers[reg2];
        registers[reg2] = val;
        if (VERBOSE) {printf("after values: reg1: %d  reg2: %d\n", registers[reg1], registers[reg2]);};
        code_index++;
        return 0;
    case OP_NLIN:
        if (VERBOSE) {printf("OP_NLIN\n");}
        printf("\n");
        code_index++;
        return 0;
    case OP_JPNT:
        if (VERBOSE) {printf("OP_JPNT\n");}
        reg1 = (code >> 16) & 0xFF;
        val = code & 0xFFFF;
        jump_points[reg1] = val;
        if (VERBOSE) {printf("jpnt: %d  value: %d actual value: %d\n", reg1, val, jump_points[reg1]);};
        code_index++;
        return 0;
    case OP_IADD:
        if (VERBOSE) {printf("OP_ADD\n");}
        reg1 = (code >> 16) & 0xFF;
        val = (code >> 8) & 0xFF;
        reg3 = code & 0xFF;
        val = registers[reg1] + val;
        registers[reg3] = val;
        if (VERBOSE) {printf("reg1: %d  reg2: %d reg3: %d result: %d\n", reg1, reg2, reg3, val);};
        code_index++;
        return 0;
    case OP_ISUB:
        if (VERBOSE) {printf("OP_SUB\n");}
        reg1 = (code >> 16) & 0xFF;
        val = (code >> 8) & 0xFF;
        reg3 = code & 0xFF;
        val = registers[reg1] - val;
        registers[reg3] = val;
        if (VERBOSE) {printf("reg1: %d  reg2: %d reg3: %d result: %d\n", reg1, reg2, reg3, val);};
        code_index++;
        return 0;
    case OP_IMUL:
        if (VERBOSE) {printf("OP_MUL\n");}
        reg1 = (code >> 16) & 0xFF;
        val = (code >> 8) & 0xFF;
        reg3 = code & 0xFF;
        val = registers[reg1] * val;
        registers[reg3] = val;
        if (VERBOSE) {printf("reg1: %d  reg2: %d reg3: %d result: %d\n", reg1, reg2, reg3, val);};
        code_index++;
        return 0;
    case OP_COPY:
        if (VERBOSE) {printf("OP_COPY\n");}
        reg1 = (code >> 16) & 0xFF;
        reg2 = (code >> 8) & 0xFF;
        registers[reg2] = registers[reg1];
        if (VERBOSE) {printf("COPIED reg1: %d  reg2: %d\n", reg1, reg2);};
        code_index++;
        return 0;
    case OP_IIJP:
        if (VERBOSE) {printf("OP_IIJP\n");}
        reg1 = (code >> 16) & 0xFF;
        val = (code >> 8) & 0xFF;
        reg2 = code & 0xFF;
        target = jump_points[reg2];
        if (VERBOSE) {printf("ii reg1: %d val: %d reg2: %d target: %d\n", registers[reg1], val, reg2, target);}
        if (registers[reg1] == val) {
            code_index = target;
            if (VERBOSE) {printf("jumping to: %d\n", target);}
        } else {
            code_index++;
            if (VERBOSE) {printf("not jumping\n");}
        }
        return 0;
    case OP_IGJP:
        if (VERBOSE) {printf("OP_IGJP\n");}
        reg1 = (code >> 16) & 0xFF;
        val = (code >> 8) & 0xFF;
        reg2 = code & 0xFF;
        target = jump_points[reg2];
        if (registers[reg1] > val) {
            code_index = target;
            if (VERBOSE) {printf("jumping to: %d\n", target);}
        } else {
            code_index++;
            if (VERBOSE) {printf("not jumping");}
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

    locate_jump_points(code, size / 4);

    int stop = 0;
    while (!stop)
    {
        stop = execute_opcode(code[code_index]);
    }
    
    return 0;
}