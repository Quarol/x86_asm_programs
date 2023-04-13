.code32
# x/9xw $sp - viewing 9 elements from stack content
# info registers <register name> - viewing register(s)
.section .data
liczba1:
    .long 0x10304008, 0x701100FF, 0x45100020, 0x08570030
	
liczba2:
	.long 0xF040500C, 0x00220026, 0x321000CB, 0x04520031

# the result(s) in hex
#    0x1, 0x00709014, 0x70330125, 0x772000EB, 0x0CA90061
#		  Ovf = 1	, Ovf = 0	, Ovf = 0   , Ovf = 0
# Ovf - overflow

# both liczba1 and liczba2 have the same number of bytes
# that number equals 16 in total
# that means 4 bytes per fragment


.section .text
.global _start
SYSEXIT = 1
EXIT_SUCCESS = 0
SYSCALL = 0x80

_start:
	# esi used as index
	movl $4, %esi
	# clearing needed registers
	xor %eax, %eax
	xor %ebx, %ebx	# setting carry to 0
	xor %ecx, %ecx
	clc


	_loop: # in range[3, 0]:
		subl $1, %esi
		
		movl liczba1(, %esi, 4), %eax
		addl liczba2(, %esi, 4), %eax
		setc %cl	# carry from adding two parts
		addl %ebx, %eax
		setc %bl	# carry from adding previous carry
		or %cl, %bl	# checking if there's carry
		pushl %eax

		cmp $0, %esi
		jne _loop

	# we also have to push last carry
	pushl %ebx
	result: # breakpoint to watch stack content

	# THE END
	movl $SYSEXIT, %eax
	movl $EXIT_SUCCESS, %ebx
	int $SYSCALL
   