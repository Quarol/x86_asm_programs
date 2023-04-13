SYSCALL = 0x80

SYSEXIT = 1
EXIT_SUCCESS = 0
SYSWRITE = 4
STDOUT = 1

SYSREAD = 3
STDIN = 0




.global _start

.data
buf: .ascii "     "
buf_len = . - buf


.text
msg: .ascii "Write text (5): \n"
msg_len = . - msg

newline: .ascii "\n"
newline_len = . - newline

key: .ascii "Karol"
key_len = . - key

not: .ascii "not "
not_len = . - not

equal: .ascii "equal"
equal_len = . - equal


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


# comparing elements and jump
movb (key), %bl
movb (buf), %al
cmp %bl, %al
jne print_not



# printing equal
equal_l:
mov $SYSWRITE, %eax
mov $STDOUT, %ebx
mov $equal, %ecx
mov $equal_len, %edx
int $SYSCALL


# printing newline
mov $SYSWRITE, %eax
mov $STDOUT, %ebx
mov $newline, %ecx
mov $newline_len, %edx
int $SYSCALL

# exiting from the programme
mov $SYSEXIT, %eax
mov $EXIT_SUCCESS, %ebx
int $SYSCALL


print_not:
mov $SYSWRITE, %eax
mov $STDOUT, %ebx
mov $not, %ecx
mov $not_len, %edx
int $SYSCALL
jmp equal_l
