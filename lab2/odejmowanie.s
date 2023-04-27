.code32
.section .data
liczba1:
    .long 0xefffffff, 0xefffffff, 0xefffffff, 0xefffffff
	
liczba2:
	.long 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff

# both liczba1 and liczba2 have the same number of bytes
# that number equals 16 in total
# that means 4 bytes per fragment


.section .text
.global _start
SYSEXIT = 1
EXIT_SUCCESS = 0
SYSCALL = 0x80

diff:
	movl liczba1(, %ecx, 4), %eax
	sbbl liczba2(, %ecx, 4), %eax
	ret

_start:
	# esi used as index
	xor %ebx, %ebx
	xor %ecx, %ecx
	movl $3, %ecx
	clc

	_loop:	# only work for i > 0	
		call diff
		pushl %eax
		loop _loop
	# for i = 0
	call diff
	pushl %eax

	# we also have to push last carry
	setc %bl
	pushl %ebx

	result: # breakpoint to watch stack content

	# THE END
	movl $SYSEXIT, %eax
	movl $EXIT_SUCCESS, %ebx
	int $SYSCALL
   