clear
gcc compiler.c -o compiled/compiler 
./compiled/compiler test.meow
gcc vm.c -lSDL2 -o compiled/vm 
./compiled/vm test.mbin
