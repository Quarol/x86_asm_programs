all: dodawanie odejmowanie

dodawanie: dodawanie.o
	ld dodawanie.o -o dodawanie

dodawanie.o: dodawanie.s
	as --gstabs dodawanie.s -o dodawanie.o

odejmowanie: odejmowanie.o
	ld odejmowanie.o -o odejmowanie

odejmowanie.o: odejmowanie.s
	as --gstabs odejmowanie.s -o odejmowanie.o
