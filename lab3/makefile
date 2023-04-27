all: fpu

fpu: fpu.o
	ld fpu.o -o fpu

fpu.o: fpu.s
	as --gstabs fpu.s -o fpu.o
