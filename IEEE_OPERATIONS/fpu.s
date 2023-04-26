.section .data
# 
num1: .float 10000000000000.1
num2: .float 3.14

# -0 for MUL and DIV
num3: .float -2.0
num4: .float 0.0

# INF for MUL
num5: .float 9999999999999999999999999.1
num6: .float 9999999999999999999999.9999999999999999999999999

# NaN for DIV
num7: .float 0.0
num8: .float 0.0

res1: .float 0.0
res2: .float 0.0
res3: .float 0.0
res4: .float 0.0


.section .text
.global _start
SYSEXIT = 1
EXIT_SUCCESS = 0
SYSCALL = 0x80

_start:
    finit

    flds num1
    flds num2
    faddp    
    fstps res1

    flds num3
    flds num4
    faddp    
    fstps res2

    flds num5
    flds num6
    faddp    
    fstps res3

    flds num7
    flds num8
    faddp    
    fstps res4     

    flds res1
    flds res2
    flds res3
    flds res4
    r1:

    # --------------------------------------------------
    finit

    flds num1
    flds num2
    fsubp   
    fstps res1

    flds num3
    flds num4
    fsubp   
    fstps res2

    flds num5
    flds num6
    fsubp   
    fstps res3

    flds num7
    flds num8
    fsubp   
    fstps res4     

    flds res1
    flds res2
    flds res3
    flds res4
    r2:

    # --------------------------------------------------
    finit

    flds num1
    flds num2
    fmulp 
    fstps res1

    flds num3
    flds num4
    fmulp 
    fstps res2

    flds num5
    flds num6
    fmulp 
    fstps res3

    flds num7
    flds num8
    fmulp 
    fstps res4     

    flds res1
    flds res2
    flds res3
    flds res4
    r3:

    # --------------------------------------------------
    finit

    flds num1
    flds num2
    fdivp
    fstps res1

    flds num3
    flds num4
    fdivp
    fstps res2

    flds num5
    flds num6
    fdivp
    fstps res3

    flds num7
    flds num8
    fdivp
    fstps res4     

    flds res1
    flds res2
    flds res3
    flds res4
    r4: 


    # THE END
    movl $SYSEXIT, %eax
    movl $EXIT_SUCCESS, %ebx
    int $SYSCALL