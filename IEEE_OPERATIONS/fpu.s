# float: 1|8|23
# e = E-127

# double: 1|11|52
# e = E-1023

# long double: 1|15|64

# denormalized when E = 0...0     => 0.abcd....
# INF when E = 1...1, M = 0...0
# NaN when E = 1...1, M != 0...0
# +/-0 when S = 0/1 E=0...0, M=0...0


.section .data
    zero: .float 0.0
    min_one: .float -1.0
    one: .float 1.0
    two: .float 2.0
	 three: .float 3.0 



# control world:
    default: .int 0x037f # exeptions: masked, round: nearest, precision: long double
    mask: .int 0x7f				# 11111111

    # precision:
    float: .int 0x0000			# 00
    double: .int 0x0200			# 10
    long_double: .int 0x0300	# 11

    # rounding:
    nearest: .int 0x0000	    # 00
    min_inf: .int 0x0400	    # 01
    plus_inf: .int 0x0800	    # 10
    truncate: .int 0x0c00	    # 11

    control_word: .int 0x0


.section .text
.global _start

SYSEXIT = 1
EXIT_SUCCESS = 0
SYSCALL = 0x80



_start:
    movl mask, %eax

    # choose precision - uncomment one
    #movl float, %ebx
    movl double, %ebx
    #movl long_double, %ebx

    or %ebx, %eax

    # choose rounding option - uncomment one
    movl nearest, %ebx
    #movl min_inf, %ebx
    #movl plus_inf, %ebx
    #movl truncate, %ebx

    or %ebx, %eax
    mov %eax, control_word
ctr: # breakpoint to view control word

    # ---------------------------------------------
    # exeptions
    finit

    fldcw control_word

    # ----------------
    # NaN on the stack: 0/0
    fld zero
    fld zero
    fdivp
    # ----------------


    # ----------------
    # -0 on the stack: 0/(-1)
    fld min_one
    fld zero
    fdivp
    # ----------------


    # ----------------
    # +0 on the stack: 0/(-1 + 2)
    fld min_one
    fld two
    faddp

    fld zero
    fdivp
    # ----------------


    # ----------------
    # +inf on the stack: 0/(2 - (-1)*1)
    fld zero

    fld one
    fld min_one
    fmulp

    fld two
    fsubp

    fdivp
    # ----------------

   
    # ----------------
    # -inf on the stack: -1/0
    fld zero
    fld min_one
    fdivp
    # ----------------

	fld three
	fld one
	fdivp


result: # breakpoint to see results



    # THE END
    movl $SYSEXIT, %eax
    movl $EXIT_SUCCESS, %ebx
    int $SYSCALL
