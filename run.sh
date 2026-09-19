clear
gcc compiler.c -o compiled/compiler 
./compiled/compiler test.meow test.mbin
gcc vm.c -lSDL2 -o compiled/vm 
./compiled/vm test.mbin
