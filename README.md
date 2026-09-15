# meowsembly

a minimal 32-bit register-based virtual architecture and toolchain. programs are written in human-readable `.meow` source code, assembled by a two-pass python compiler into a compact binary format (`.mbin`), and executed by a fast virtual processor written in c.

---

## architecture overview

* **registers:** 256 general-purpose registers (`R0`–`R255`), each storing a signed 32-bit integer.
* **jump table:** 256 dedicated jump targets (`0`–`255`) resolved automatically via named labels during pass 1.
* **word size:** fixed 32-bit (4-byte) instructions encoded in little-endian byte order.
* **memory model:** register-indirect addressing via pointer swapping (`PSWP`) allows registers to act as dynamic memory pointers and data buffers simultaneously.

---

## instruction encoding

every instruction occupies exactly 4 bytes (32 bits):

```
 31        24 23        16 15         8 7          0  (bit index)
+------------+------------+------------+------------+
|   opcode   |   arg 1    |   arg 2    |   arg 3    |
+------------+------------+------------+------------+


```

* **immediate values:** adding a dot prefix (e.g., `.10`) signals an immediate literal value rather than a register lookup. the assembler automatically promotes standard opcodes to their immediate counterparts (`_iadd`, `_isub`, `_imul`, `_iijp`, `_igjp`).
* **constants:** `LOAD` uses the full lower 16 bits (`Arg 2` + `Arg 3`) to allow loading 16-bit integers up to `65535` in a single word.

---

## instruction set

| opcode | mnemonic & syntax | description |
| --- | --- | --- |
| `0` | `LOAD dest imm` | loads a 16-bit immediate value into register `dest`. |
| `1` | `JUMP label` | unconditionally jumps to named `label`. |
| `2` | `PNUM reg` | prints the numeric value stored in `reg`. |
| `3` | `PCHR reg` | prints the ascii character represented by `reg`. |
| `4` | `SWAP reg1 reg2` | swaps the values of `reg1` and `reg2` directly. |
| `5` | `ADD src1 src2 dest` | adds `src1` + `src2` and stores result in register `dest`. |
| `6` | `SUB src1 src2 dest` | subtracts `src1` - `src2` and stores result in register `dest`. |
| `7` | `MUL src1 src2 dest` | multiplies `src1` * `src2` and stores result in register `dest`. |
| `8` | `END` | terminates execution. |
| `9` | `IJMP reg1 reg2 label` | jumps to `label` if `registers[reg1] == registers[reg2]`. |
| `10` | `GJMP reg1 reg2 label` | jumps to `label` if `registers[reg1] > registers[reg2]`. |
| `11` | `_omit` | internal nop emitted for empty lines and comments (preserves line-address parity). |
| `12` | `PSWP ptr1 ptr2` | **pointer swap:** treats `ptr1` and `ptr2` as pointers; swaps `registers[registers[ptr1]]` and `registers[registers[ptr2]]`. |
| `13` | `NLIN` | prints a newline character `\n`. |
| `14` | `: label_name` | declares a named jump label. |
| `18` | `COPY src dest` | copies the value from `src` into `dest` (`registers[dest] = registers[src]`). |

### auto-promoted immediate opcodes

you don't need to call these directly; typing a dot literal (like `.5`) automatically promotes the instruction:

* `15 (_iadd)`: `ADD reg1 .imm dest` $\rightarrow$ `dest = reg1 + imm`
* `16 (_isub)`: `SUB reg1 .imm dest` $\rightarrow$ `dest = reg1 - imm`
* `17 (_imul)`: `MUL reg1 .imm dest` $\rightarrow$ `dest = reg1 * imm`
* `19 (_iijp)`: `IJMP reg1 .imm label` $\rightarrow$ jump if `reg1 == imm`
* `20 (_igjp)`: `GJMP reg1 .imm label` $\rightarrow$ jump if `reg1 > imm`

---

## toolchain & usage

### 1. assemble

translate `.meow` source code into binary bytecode:

```bash
python parser_3.py


```

this reads `test.meow` and generates the raw binary file `test.mbin`.

### 2. compile and run executor

```bash
gcc -O2 executor_3.c -o executor
./executor


```