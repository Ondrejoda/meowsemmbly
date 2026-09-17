clear
gcc -o compiled/compiler compiler.c
./compiled/compiler test.meow
gcc -o compiled/vm vm.c
./compiled/vm test.mbin
