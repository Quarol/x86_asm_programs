#p/t $fstat
#p/t $fctrl

#suffix: s-single, t-double, I-long double

#|0-IM|1-DM|2-ZM|3-OM|4-UM|5-PM|6|7|8,9-PC|a,b-RC|c|d|e|
#IM - invalid mask
#DM - denormalized mask
#ZM - zero divide mask
#OM - overflow mask
#UM - underflow mask
#PM - precision mask
#PC - precision ctr
#RC - round ctr

.section .data  # 
    ctrm_to_zero: .int 0b1111000011111111 #float, do najblizszej (and)
    ctrm_double:  .int 0b0000001000000000 #double (or)
    ctrm_ldouble: .int 0b0000001100000000 #long double (or)
    ctrm_down:    .int 0b0000010000000000# w dół (or)
    ctrm_up:      .int 0b0000100000000000# w up (or)
    ctrm_cut:     .int 0b0000110000000000# obcinanie(or)

    ctrw: .int 0b1111111111000000
    
    min_one: .float -1
    zero: .float 0.00
    two: .float 2.00
    
    to_round_1: .float 1.0e+38
    to_round_2: .float 1.0e-38


.section .text
.globl _start

#Deklaracje stalych
SYSEXIT = 1
EXIT_SUCCESS = 0
INTERUPT = 0x80


_start:
    finit # initialize FPU

    #float + to closest
    call BASE_CW

    # +zero (0 in)
    flds zero
    
    # -zero (0 * -1)
    flds min_one
    fmul %st(1)

    # +inf (2 / 0)
    flds two
    fdiv %st(2)

    # -inf (2 / -0)
    flds two
    fdiv %st(2)

    # Nan ( sqrt(-1) )
    flds min_one
    fsqrt
exe:

    #Double + round to -inf
    call DOUBLE_P_CW
    call DOWN_R_CW
prec:

    call BASE_CW
    call LDOUBLE_P_CW
    call UP_R_CW

#Konczy program
END:
    hlt
    movl $SYSEXIT, %eax
    movl $EXIT_SUCCESS, %ebx
    int $INTERUPT

#float + round to closest
BASE_CW:
    fnstcw ctrw
    movw ctrw, %bx
    and ctrm_to_zero, %bx
    movw %bx, ctrw
    fldcw ctrw
    ret
    
DOUBLE_P_CW:
    fnstcw ctrw
    movw ctrw, %bx
    xor ctrm_double, %bx
    movw %bx, ctrw
    fldcw ctrw
    ret


LDOUBLE_P_CW:
    fnstcw ctrw
    movw ctrw, %bx
    xor ctrm_ldouble, %bx
    movw %bx, ctrw
    fldcw ctrw
    ret


DOWN_R_CW:
    fnstcw ctrw
    movw ctrw, %bx
    xor ctrm_down, %bx
    movw %bx, ctrw
    fldcw ctrw
    ret

UP_R_CW:
    fnstcw ctrw
    movw ctrw, %bx
    xor ctrm_up, %bx
    movw %bx, ctrw
    fldcw ctrw
    ret

CUT_R_CW:
    fstcw ctrw
    movw ctrw, %bx
    xor ctrm_cut, %bx
    movw %bx, ctrw
    fldcw ctrw
    ret