#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

const int VERBOSE = 0;
const int SUPER_VERBOSE = 0;

const int MAX_STRING_LENGTH = 100;

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

struct opcode {
    int opcode;
    int arg1;
    int arg2;
    int arg3;
};

char opcode_names[22][5] = {"LOAD", "JUMP", "PNUM", "PCHR", "SWAP", "ADD", "SUB", "MUL", "END", "IJMP", "GJMP", "omit", "PSWP", "NLIN", "iadd", "isub", "imul", "COPY", "iijp", "igjp", "UJMP", "iujp"};
//                           0       1       2       3       4       5      6      7      8      9       10      11      12      13      14      15      16      17      18      19      20      21

int iops[22] = {0, 0, 0, 0, 0, 14, 15, 16, 0, 18, 19, 0, 0, 0, 0, 0, 0, 0, 0, 0, 21, 0};

char *grab_jump_point(char line[]) {
    char *rtn = malloc(MAX_STRING_LENGTH);
    if (line[0] != ':') {
        strcpy(rtn, "");
        return rtn;
    }

    char jump_point[MAX_STRING_LENGTH];
    
    int current_line_index = 2;
    int jump_point_index = 0;
    char current_char = line[current_line_index];
    while (current_char != '\n' && current_char != 0) {
        jump_point[jump_point_index] = line[current_line_index];
        jump_point_index++;
        current_line_index++;
        current_char = line[current_line_index];
    }

    jump_point[jump_point_index] = 0;

    strcpy(rtn, jump_point);

    return rtn;
}

struct opcode tokenize(char line[], char jump_point_names[][MAX_STRING_LENGTH], int jump_point_indexes[], int jump_point_count) {
    struct opcode op;

    // snipe those omits
    op.opcode = OP_OMIT;

    if (line[0] == '/' || line[0] == '\n' || line[0] == ':') {
        op.arg1 = 0;
        op.arg2 = 0;
        op.arg3 = 0;
        return op;
    }

    // tokens stuff
    char tokens[4][MAX_STRING_LENGTH];
    int current_token_index = 0;
    int current_token_char_index = 0;
    int has_immediate = 0;

    char current_token[MAX_STRING_LENGTH];
    memset(current_token,0,sizeof(current_token));

    for (int current_line_index = 0; current_line_index < MAX_STRING_LENGTH; current_line_index++) {
        char chr = line[current_line_index];
        if (chr == '\n' || chr == 0) {
            if (SUPER_VERBOSE) {printf("loading token index: %d\n", current_token_index);}
            strcpy(tokens[current_token_index], current_token);
            break;
        } else if (chr == ' ') {
            // move to next token slot
            if (SUPER_VERBOSE) {printf("loading token index: %d\n", current_token_index);}
            strcpy(tokens[current_token_index], current_token);
            memset(current_token,0,sizeof(current_token));
            current_token_char_index = 0;
            current_token_index++;
        } else {
            if (chr == '.') {
                has_immediate = 1;
            } else {
                current_token[current_token_char_index] = chr;
                current_token_char_index++;
            }
        }
        if (SUPER_VERBOSE) {printf("current_char: %c\n", chr);}
        if (SUPER_VERBOSE) {printf("current_token: %s\n", current_token);}

    }

    // fill in empty tokens to avoid junk
    if (current_token_index != 3) {
        for (int i = current_token_index + 1; i < 4; i++) {
            strcpy(tokens[i], "");
        }
    }

    for (int i = 0; i < 23; i++) {
        if (strcmp(tokens[0], opcode_names[i]) == 0) {
            if (has_immediate == 1) {
                op.opcode = iops[i];
            } else {
                op.opcode = i;
            }
            break;
        }
    }

    int replaced_jp1 = 0;
    int replaced_jp2 = 0;
    int replaced_jp3 = 0;
    for (int i = 0; i < jump_point_count; i++) {
        if (strcmp(tokens[1], jump_point_names[i]) == 0) {
            op.arg1 = jump_point_indexes[i];
            replaced_jp1 = 1;
        }
        if (strcmp(tokens[2], jump_point_names[i]) == 0) {
            op.arg2 = jump_point_indexes[i];
            replaced_jp2 = 1;
        }
        if (strcmp(tokens[3], jump_point_names[i]) == 0) {
            op.arg3 = jump_point_indexes[i];
            replaced_jp3 = 1;
        }
    }

    if (!replaced_jp1) {
        op.arg1 = atoi(tokens[1]);
    }
    if (!replaced_jp2) {
        op.arg2 = atoi(tokens[2]);
    }
    if (!replaced_jp3) {
        op.arg3 = atoi(tokens[3]);
    }

    return op;
}

int main() {
    // loading file
    FILE *file;
    file = fopen("test.meow", "r");

    // establishing counts
    int line_count = 0;
    int jump_point_count = 0;
    char myString[MAX_STRING_LENGTH];

    // counting
    while(fgets(myString, MAX_STRING_LENGTH, file)) {
        if (strstr(myString, ":") != NULL) {
            jump_point_count++;
        }
        line_count++;
    }

    fseek(file, 0, SEEK_SET);

    // defining arrays
    char code[line_count][MAX_STRING_LENGTH];
    char jump_point_names[jump_point_count][MAX_STRING_LENGTH];
    int jump_point_indexes[jump_point_count];
    int current_line = 0;

    // loading stuff into arrays
    while(fgets(myString, MAX_STRING_LENGTH, file)) {
        strcpy(code[current_line], myString);
        current_line++;
    }

    fclose(file); 

    // grabbing all jump points
    char line[MAX_STRING_LENGTH];
    int jump_point_index = 0;
    for (int i = 0; i < line_count; i++) {
        strcpy(line, code[i]);
        char *jp = grab_jump_point(line);
        if (strlen(jp) > 0) {
            strcpy(jump_point_names[jump_point_index], jp);
            jump_point_indexes[jump_point_index] = i;
            jump_point_index++;
        }
        free(jp);
    }

    
    file = fopen("test.mbin", "wb");

    for (int i = 0; i < line_count; i++) {
        strcpy(line, code[i]);
        struct opcode op = tokenize(line, jump_point_names, jump_point_indexes, jump_point_count);
        if (VERBOSE) {printf("===\nline %d: %sopcode: %d arg1: %d arg2: %d arg3: %d\n===\n", i, code[i], op.opcode, op.arg1, op.arg2, op.arg3);};

        uint32_t to_write = 0;
        
        if (op.opcode == OP_LOAD) {
            to_write = (op.opcode << 24) | (op.arg1 << 16) | op.arg2;
        } else {
            to_write = (op.opcode << 24) | (op.arg1 << 16) | (op.arg2 << 8) | op.arg3;
        }
        fwrite(&to_write, 4, 1, file);
    }

    fclose(file);

    return 0;
}