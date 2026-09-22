#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>

#define VERBOSE 0
#define VERBOSEJP 0

#define REGISTER_SIZE 256
#define MEMORY_SIZE 65535
#define CALL_STACK_SIZE 1024

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
    OP_IUJP,
    OP_DIV,
    OP_IDIV,
    OP_MOD,
    OP_IMOD,
    OP_PCPY,
    OP_READ,
    OP_WRIT,
    OP_RAND,
    OP_SPAC,
    OP_CALL,
    OP_RTN,
    OP_ERR,
    OP_ISDL,
    OP_SCOL,
    OP_DRAW,
    OP_FLSH,
    OP_ISKP,
    OP_IISK,
    OP_USKP,
    OP_IUSK,
    OP_GSKP,
    OP_IGSK
};

int registers[REGISTER_SIZE];
int memory[MEMORY_SIZE];

int call_stack[CALL_STACK_SIZE];    
int csp = 0;

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

int code_index = 0;

int execute_opcode(uint32_t code) {
    int opcode = ((code >> 24) & 0xFF);
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
    case OP_DIV:
        if (VERBOSE) {printf("OP_DIV\n");}
        if (registers[arg2] == 0) {
            printf("speed i be watching your stream bruh why you tryin not to laugh bruh");
            return 1;
        }
        val = registers[arg1] / registers[arg2];
        registers[arg3] = val;
        if (VERBOSE) {printf("arg1: %d  arg2: %d arg3: %d result: %d\n", arg1, arg2, arg3, val);};
        code_index++;
        return 0;
    case OP_IDIV:
        if (VERBOSE) {printf("OP_IDIV\n");}
        if (arg2 == 0) {
            printf("speed i be watching your stream bruh why you tryin not to laugh bruh");
            return 1;
        }
        val = registers[arg1] / arg2;
        registers[arg3] = val;
        if (VERBOSE) {printf("arg1: %d  arg2: %d arg3: %d result: %d\n", arg1, arg2, arg3, val);};
        code_index++;
        return 0;
    case OP_MOD:
        if (VERBOSE) {printf("OP_MOD\n");}
        val = registers[arg1] % registers[arg2];
        registers[arg3] = val;
        if (VERBOSE) {printf("arg1: %d  arg2: %d arg3: %d result: %d\n", arg1, arg2, arg3, val);};
        code_index++;
        return 0;
    case OP_IMOD:
        if (VERBOSE) {printf("OP_IMOD\n");}
        val = registers[arg1] % arg2;
        registers[arg3] = val;
        if (VERBOSE) {printf("arg1: %d  arg2: %d arg3: %d result: %d\n", arg1, arg2, arg3, val);};
        code_index++;
        return 0;
    case OP_PCPY:
        if (VERBOSE) {printf("OP_PCPY\n");}
        registers[arg2] = registers[registers[arg1]];
        if (VERBOSE) {printf("COPIED arg1: %d  arg2: %d\n", arg1, arg2);};
        code_index++;
        return 0;
    case OP_READ:
        if (VERBOSE) {printf("OP_READ\n");}
        val = memory[registers[arg1]];
        registers[arg2] = val;
        code_index++;
        if (VERBOSE) {printf("READ addr: %d  val: %d\n", arg1, val);};
        return 0;
    case OP_WRIT:
        if (VERBOSE) {printf("OP_WRIT\n");}
        val = registers[arg1];
        memory[registers[arg2]] = val;
        code_index++;
        if (VERBOSE) {printf("WRITE addr: %d  val: %d\n", arg1, val);};
        return 0;
    case OP_RAND:
        if (VERBOSE) {printf("OP_RAND\n");}
        val = rand();
        val = val % (registers[arg2] - registers[arg1] + 1);
        val += registers[arg1];
        registers[arg3] = val;
        if (VERBOSE) {printf("arg1: %d  arg2: %d arg3: %d result: %d\n", arg1, arg2, arg3, val);};
        code_index++;
        return 0;
    case OP_SPAC:
        if (VERBOSE) {printf("OP_SPAC\n");}
        printf(" ");
        code_index++;
        return 0;
    case OP_CALL:
        if (VERBOSE) {printf("OP_CALL\n");}
        call_stack[csp++] = code_index + 1;
        code_index = arg1;
        if (VERBOSE) {printf("calling: %d\n", arg1);};
        return 0;
    case OP_RTN:
        if (VERBOSE) {printf("OP_RTN\n");}
        code_index = call_stack[--csp];
        if (VERBOSE) {printf("returning\n");};
        return 0;
    case OP_ERR:
        if (VERBOSE) {printf("OP_ERR\n");}
        printf("oops! something went wrong (you probably tried to use an opcode that doesnt exist or use an alias you didnt declare...)\n");
        return 1;
    case OP_ISDL:
        if (VERBOSE) {printf("OP_ISDL\n");}
        SDL_Init(SDL_INIT_VIDEO);
        window = SDL_CreateWindow("meowsemmbly", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
                                registers[arg1], registers[arg2], 0);
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        code_index++;
        return 0;
    case OP_SCOL:
        if (VERBOSE) {printf("OP_SCOL\n");}
        if (window == NULL) {
            printf("SDL not present!\n");
            return 1;
        }
        SDL_SetRenderDrawColor(renderer, registers[arg1], registers[arg2], registers[arg3], 255);
        code_index++;
        return 0;
    case OP_DRAW:
        if (VERBOSE) {printf("OP_DRAW\n");}
        if (window == NULL) {
            printf("SDL not present!\n");
            return 1;
        }
        SDL_RenderDrawPoint(renderer, registers[arg1], registers[arg2]);
        code_index++;
        return 0;
    case OP_FLSH:
        if (VERBOSE) {printf("OP_FLSH\n");}
        if (window == NULL) {
            printf("SDL not present!\n");
            return 1;
        }
        SDL_RenderPresent(renderer);

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                return 1;
            }
        }
        code_index++;
        return 0;
    case OP_ISKP:
        if (VERBOSE) {printf("OP_ISKP\n");}
        if (registers[arg1] == registers[arg2]) {
            code_index++;
            code_index++;
        } else {
            code_index++;
        }
        return 0;
    case OP_IISK:
        if (VERBOSE) {printf("OP_IISK\n");}
        if (registers[arg1] == arg2) {
            code_index++;
            code_index++;
        } else {
            code_index++;
        }
        return 0;
    case OP_USKP:
        if (VERBOSE) {printf("OP_USKP\n");}
        if (registers[arg1] != registers[arg2]) {
            code_index++;
            code_index++;
        } else {
            code_index++;
        }
        return 0;
    case OP_IUSK:
        if (VERBOSE) {printf("OP_IUSK\n");}
        if (registers[arg1] != arg2) {
            code_index++;
            code_index++;
        } else {
            code_index++;
        }
        return 0;
    case OP_GSKP:
        if (VERBOSE) {printf("OP_GSKP\n");}
        if (registers[arg1] > registers[arg2]) {
            code_index++;
            code_index++;
        } else {
            code_index++;
        }
        return 0;
    case OP_IGSK:
        if (VERBOSE) {printf("OP_IGSK\n");}
        if (registers[arg1] > arg2) {
            code_index++;
            code_index++;
        } else {
            code_index++;
        }
        return 0;
    default:
        return 0;
    }
}

int main(int argc, char **argv) {
    srand(time(NULL));

    FILE *file;
    file = fopen(argv[1], "rb");
    fseek(file, 0, SEEK_END);
    int size = ftell(file);
    fseek(file, 0, SEEK_SET);

    uint32_t *code = malloc(size);

    fread(code, 4, size / 4, file);
    fclose(file);

    int stop = 0;
    while (!stop)
    {
        stop = execute_opcode(code[code_index]);
        if (code_index >= size / 4) {
            stop = 1;
            printf("code overflow - dont forget END!\n");
        }
    }

    if (renderer != NULL) {
        int running = 1;
        while (running == 1) {
            SDL_Event event;
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT) {
                    running = 0;
                }
            }  
        }
    }
    
    free(code);

    return 0;
}