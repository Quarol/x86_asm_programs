all: fpu m

fpu: fpu.o
	ld fpu.o -o fpu

fpu.o: fpu.s
	as --gstabs fpu.s -o fpu.o

m: m.o
	ld m.o -o m

m.o: m.s
	as --gstabs m.s -o m.o