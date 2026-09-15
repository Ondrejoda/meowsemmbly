# meowsemmbly

- stupid interpreted assembly
- its silly

## data
- there is no stack or heap (YET), all you get is 256 registries

## opcodes

- most opcodes use registries for operations
- the exc

## list of opcodes

- LOAD x y
    - loads the number `y` (which is a two byte number) into registry `x`
- JUMP x
    - jumps to the line of code specified from registry `x` (also counts empty lines and comments! why? because then its super simple to determine where you actually want to jump)
- PNUM x
    - prints whats on registry `x` as a number
- PCHR x
    - prints whats on registry `x` as a char
- SWAP x y
    - swaps contents of registries `x` and `y`
- ADD x y z
    - adds whats on registry `x` and `y` and puts the result in registry `z`
    - x + y = z
- SUB x y z
    - substracts whats on registry `y` from registry `x` and puts the result in registry `z`
    - x - y = z
- MUL x y z
    - multiplies whats on registry `x` and `y` and puts the result in registry `z`
    - x * y = z
- END
    - ends program
- IJMP x y z
    - jumps to line from registry `z` if values on `x` and `y` are equal
    - if x == y then jump to z
- OMIT
    - dont use, parser autoinserts for blank lines and comments
- PSWP x y
    - same as SWAP, but instead of swapping registries directly, it takes the registries it should swap from the registries specified (`x`, `y`)
    - this is neccesary for more dynamic behaviours
- GJMP x y z
    - same as IJMP, but instead of checking if `x` and `y` are equal, it checks if `x` is greater than `y`
    - if x > y then jump to z
- NLIN
    - just prints a newline; its more convenient than saving a newline in a registry and then printing it using PCHR