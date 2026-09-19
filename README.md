# meowsemmbly

a semi-esoteric assembly-like language programmed in C. i created it with the intention of learning C and low-level programming. i never coded anything in assembly, all the opcodes were designed purely with my creativity, which explains why it is a clumsy and inconvenient language to use.

---

## how does meowsemmbly work?

meowsemmbly (which i will refer to as ms further) has a structure similar to java or c# - a program starts with a .meow source code file. that file then gets compiled by the compiler to a .mbin file, which can be run by the vm.

a program in ms has access to two things - those being a group of 256 registrers and a memory with 65535 addresses. each register or address can store a C `int`.

ms does have a call stack, used by opcodes `CALL` and `RTN` (more on those in the opcodes section).

ms also has access to SDL2, albeit somewhat limited - all a program can do is create a window and draw pixel by pixel (more seen in the opcodes section).

---

## how does syntax look?

each line corresponds with an opcode. each opcode can have up to 3 arguments, where each argument is exactly one byte in size (with the sole exception of `OP_LOAD`, which will be explained later). this allows each command to fit into 4 bytes, thus simplifying the vm.

there are three exceptions to this, however - comments, jump point markers and aliases.
- comments - they start with `/`, but i usually just write `//` out of habit.
- jump point markers - these mark points where opcodes such as `JUMP`, `IJMP` or `CALL` jump to. they are marked with a colon and look like this: `: jump_point`
- aliases - these allow the user to name registers for improved readability of code. they are marked with a `@` and look like this: `@ alias 1`

also, some selected opcodes are capable of handling both registers and immediate values as arguments - they will be named in the opcodes section. an immediate value is marked like this: `.100`

here is an example of how ms code looks:

```
@ pos1 0
@ pos2 1
@ temp 2

@ loop 10

LOAD pos1 0
LOAD pos2 1
LOAD loop 0

// main loop
: main

ADD pos1 pos2 temp
SWAP pos1 pos2
SWAP pos2 temp

PNUM pos1
NLIN

ADD loop .1 loop
UJMP loop .10 main

// end
NLIN
END
```

---

## opcodes explained

| opcode | mnemonic & syntax           | can handle immutables | description                                                                                                                                                                                                          |
| ------ | --------------------------- | --------------------- | -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| `0`    | `LOAD dest imm`             | has to                | loads a 16-bit immediate value into register `dest`. the compiler glues `arg2` and `arg3` together for this opcode, thus allowing the user to exceed the `256` number limit. it is the only opcode that can do this. |
| `1`    | `JUMP label`                | no                    | unconditionally jumps to jump point `label`.                                                                                                                                                                         |
| `2`    | `PNUM reg`                  | no                    | prints the numeric value stored in `reg`.                                                                                                                                                                            |
| `3`    | `PCHR reg`                  | no                    | prints the ascii character represented by `reg`.                                                                                                                                                                     |
| `4`    | `SWAP reg1 reg2`            | no                    | swaps the contents of `reg1` and `reg2` directly.                                                                                                                                                                    |
| `5`    | `ADD src1 src2/imm2 dest`   | yes                   | adds `src1` + `src2` and stores result in register `dest`.                                                                                                                                                           |
| `6`    | `SUB src1 src2/imm2 dest`   | yes                   | subtracts `src1` - `src2` and stores result in register `dest`.                                                                                                                                                      |
| `7`    | `MUL src1 src2/imm2 dest`   | yes                   | multiplies `src1` * `src2` and stores result in register `dest`.                                                                                                                                                     |
| `8`    | `END`                       | no                    | terminates execution.                                                                                                                                                                                                |
| `9`    | `IJMP reg1 reg2/imm2 label` | yes                   | jumps to `label` if `registers[reg1] == registers[reg2]` (or `registers[reg1] == imm2`.                                                                                                                              |
| `10`   | `GJMP reg1 reg2/imm2 label` | yes                   | jumps to `label` if `registers[reg1] > registers[reg2]` (or `registers[reg1] > imm2`.                                                                                                                                |
| `12`   | `PSWP ptr1 reg2`            | no                    | **pointer swap:** treats `ptr1` as a pointer; swaps `registers[registers[ptr1]]` and `registers[reg2]`.                                                                                                              |
| `13`   | `NLIN`                      | no                    | prints a newline character `\n`.                                                                                                                                                                                     |
| `18`   | `COPY src dest`             | no                    | copies the value from `src` into `dest` (`registers[dest] = registers[src]`).                                                                                                                                        |
| `21`   | `UJMP reg1 reg2/imm2 label` | yes                   | jumps to `label` if `registers[reg1] != registers[reg2]` (or `registers[reg1] != imm2`.                                                                                                                              |
| `23`   | `DIV src1 src2/imm2 dest`   | yes                   | divides `src1` / `src2` and stores result in register `dest`.                                                                                                                                                        |
| `25`   | `MOD src1 src2/imm2 dest`   | yes                   | modulos `src1` % `src2` and stores result in register `dest`.                                                                                                                                                        |
| `27`   | `PCPY ptr1 reg2`            | no                    | **pointer copy:** treats `ptr1` as a pointer; copies `registers[registers[ptr1]]` to `registers[reg2]`.                                                                                                              |
| `28`   | `READ src dest`             | no                    | reads memory at address specified by `src` register and stores it in register `dest`                                                                                                                                 |
| `29`   | `WRIT src dest`             | no                    | writes to memory contents of register `src` to address specified by register `dest`                                                                                                                                  |
| `30`   | `RAND min max dest`         | no                    | generates a random number with the range specified by values at registers `max` and `min` and stores the number at register `dest`                                                                                   |
| `31`   | `SPAC`                      | no                    | prints a space.                                                                                                                                                                                                      |
| `32`   | `CALL jump_point`           | no                    | calls a jump point as a function - similar to `JUMP`, however, the previous position gets stored in the call stack and can be returned to using `RTN`                                                                |
| `33`   | `RTN`                       | no                    | jumps to the point last stored in the call stack; used in functions called by `CALL`                                                                                                                                 |
| `35`   | `ISDL width height`         | no                    | initiates SDL; sets the width and height of the window to the contents of the `width` and `height` registers                                                                                                         |
| `36`   | `SCOL r g b`                | no                    | sets the draw color to the contents of registers `r`, `g` and `b`                                                                                                                                                    |
| `37`   | `DRAW x y`                  | no                    | draws a pixel at the position specified by the contents of registers `x` and `y`; the color is set by `SCOL` instead                                                                                                 |
| `38`   | `FLSH`                      | no                    | renders the screen; keep in mind that this wipes the screen after!                                                                                                                                                   |
| `39`   | `ISKP reg1 reg2/imm2`       | yes                   | skips the next line of code if `registers[reg1] == registers[reg2]` (or `registers[reg1] == imm2`.                                                                                                                   |
| `41`   | `USKP reg1 reg2/imm2`       | yes                   | skips the next line of code if `registers[reg1] != registers[reg2]` (or `registers[reg1] != imm2`.                                                                                                                   |
| `43`   | `GSKP reg1 reg2/imm2`       | yes                   | skips the next line of code if `registers[reg1] > registers[reg2]` (or `registers[reg1] > imm2`.                                                                                                                     |

there are some secret opcodes that the compiler uses under the hood, but i wont mention those here :3

---

## how to compile and run ms code?

the `run.sh` in the root of the repository works just fine. however, if you wish to not use it, you can do as follows:
1) run the compiler: `./compiler source.meow binary.mbin`
2) run the vm: `./vm binary.mbin`

for compilation, keep in mind that only the vm needs SDL2.

---