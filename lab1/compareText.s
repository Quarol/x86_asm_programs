# Ograniczenia: 
# - "al" jest rejestrem 8 bitowym
# - jeden znak ascii ma 8 bitow
# - dlatego nalezy porownywac znak po znaku


SYSCALL = 0x80

SYSEXIT = 1
EXIT_SUCCESS = 0
SYSWRITE = 4
STDOUT = 1

SYSREAD = 3
STDIN = 0


.global _start

.data
buf: .ascii "   "
buf_len = . - buf


.text
msg: .ascii "The key value: tak\nWrite text (3):\n"
msg_len = . - msg

key: .ascii "tak"
key_len = . - key

not: .ascii "not "
not_len = . - not

equal_newline: .ascii "equal\n"
equal_len = . - equal_newline


_start:
# printing message
mov $SYSWRITE, %eax
mov $STDOUT, %ebx
mov $msg, %ecx
mov $msg_len, %edx
int $SYSCALL

# reading input text 
mov $SYSREAD, %eax
mov $STDIN, %ebx
mov $buf, %ecx
mov $buf_len, %edx
int $SYSCALL


# comparing elements
xor %ecx, %ecx		# setting the counter
xor %edx, %edx		# clearing key_len holder register
mov $key_len, %edx	# putting key_len to a register

compare_char:		# looping through all chars
movb buf(%ecx), %al
movb key(%ecx), %bl
cmp %al, %bl
jne print_not

inc %ecx
cmp %edx, %ecx		# checking if all chars compared
jne compare_char

jmp print_equal


print_not:
mov $SYSWRITE, %eax
mov $STDOUT, %ebx
mov $not, %ecx
mov $not_len, %edx
int $SYSCALL


#printing equal with newline
print_equal:
mov $SYSWRITE, %eax
mov $STDOUT, %ebx
mov $equal_newline, %ecx
mov $equal_len, %edx
int $SYSCALL


# exiting from the programme
mov $SYSEXIT, %eax
mov $EXIT_SUCCESS, %ebx
int $SYSCALL