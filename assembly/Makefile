#makefile

hello:
	gcc -c hello.S && ld -o hello hello.o

prime:
	yasm -g dwarf2 -f elf64 example.asm -l example.lst

clean:
	rm *.o
