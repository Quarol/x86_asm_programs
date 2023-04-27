all: hello writeText compareText

hello: hello.o
	ld hello.o -o hello -m elf_i386

hello.o: hello.s
	as hello.s -o hello.o --32

writeText: writeText.o
	ld writeText.o -o writeText -m elf_i386

writeText.o: writeText.s
	as writeText.s -o writeText.o --32

compareText: compareText.o
	ld compareText.o -o compareText -m elf_i386

compareText.o: compareText.s
	as compareText.s -o compareText.o --32
