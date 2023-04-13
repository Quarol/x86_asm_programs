all: addition subtraction

addition: addition.o
	ld addition.o -o addition

dodawanie.o: addition.s
	as --gstabs addition.s -o addition.o

subtraction: subtraction.o
	ld subtraction.o -o subtraction

subtraction.o: subtraction.s
	as --gstabs subtraction.s -o subtraction.o
