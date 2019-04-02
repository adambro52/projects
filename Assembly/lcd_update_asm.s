	.text
	.globl	set_tod_from_secs
	.globl	set_display_bits_from_tod
	.globl	lcd_update
set_tod_from_secs:

    cmpq $86400, %rdi # make sure seconds is less than 86400
    jge .else_main
    cmpq $0, %rdi # make sure seconds is greater than 0
    jle .else_main

    #calculate hours
    movq %rdi, %rax # set up dividend
    cqto # extend sign
    movq $3600, %rcx # set up divisor
    idivq %rcx # divide
    movl %eax, 0(%rsi) # put quotient hours into its tod field
    movq %rax, %r10
    movl %eax, %r11d #r11d holds the hours


    # start calculation for minutes
    imul $3600, %rax
    movq %rdi, %r9 # temp var for total seconds
    subq %rax, %r9 # subtract, eax now needs to be divided by 60 to get minutes
    movq %r9, %rax # set up dividend
    cqto # extend sign
    movq $60, %rcx # set up divisor
    idivq %rcx # divide
    movl %eax, 2(%rsi) # rax is now minutes

    # start calculation for seconds
    imul $60, %rax
    imul $3600, %r10
    addq %rax, %r10 # ecx is now total seconds from hours and minutes
    movq %rdi, %rax # eax is now start seconds
    subq %r10, %rax # get remaining seconds
    movl %eax, 4(%rsi) # put it in the struct


    # check if hours is greater than 12
.cmp:
    cmpl $12, %r11d
    jle .else1 # makes sure that the next part is executed when hours greater than 12
    subl $12, 0(%rsi) # subtracts 12 hrs so it is not 24 hr time
    movw $1, 6(%rsi) # set ispm to 1
    jmp .return0
.else1: # enters if hours <= 12
    cmpl $12, %r11d
   # executes next lines if hours = 12
    jne .else2
    movw $1, 6(%rsi) # set ispm to 1
    jmp .return0
.else2: # enters here if hours is less than 12 only
    cmpl $0, %r11d
    jne .else3
    movl 2(%rsi), %r11d
    movl $12, 0(%rsi)
    movl %r11d, 2(%rsi)
    movw $0, 6(%rsi) # set ispm to 0
    jmp .return0
.else3: # enters here if hours != 0
    movw $0, 6(%rsi) # set ispm to 0
.return0: # success
    movq $0, %rax
    ret
.return1: # error
    movq $1, %rax
    ret
.else_main:
    # enters here if start seconds is out of bounds or 0
    cmpq $0, %rdi
    jne .return1 # means that start seconds is out of bounds
    movl $12, 0(%rsi)
    movl $0, 2(%rsi)
    movl $0, 4(%rsi)
    movw $0, 6(%rsi)
    jmp .return0

.section .data
    bitbask:
    .int 0b0111111
    .int 0b0000110
    .int 0b1011011
    .int 0b1001111
    .int 0b1100110
    .int 0b1101101
    .int 0b1111101
    .int 0b0000111
    .int 0b1111111
    .int 0b1101111
    .int 0b0000000

.text
set_display_bits_from_tod: # takes in a tod struct and an int pointer to the display
if:
    movq %rdi, %r8
    andq $0xffff, %r8   # r8 now has hours
    movq %rdi, %r9
    sarq $16, %r9
    andq $0xffff, %r9   # r9 now has minutes
    movq %rdi, %r10
    sarq $32, %r10
    andq $0xffff, %r10  # r10 now has seconds
    movq %rdi, %r11
    sarq $48, %r11
    andq $0xff, %r11  # r11 now has ispm

    # checks if the time of day is in bounds
    cmpq $0, %r8
    jl .error
    cmpq $12, %r8
    jg .error
    cmpq $0, %r9
    jl .error
    cmpq $59, %r9
    jg .error
    cmpq $0, %r10
    jl .error
    cmpq $59, %r10
    jg .error

    movl $0, (%rsi)   # set display to 0

    cmpq $1, %r11     # see if it is am or pm
    jne  .setam
    orw  $2, (%rsi) # set display for ispm
    shll $7, (%rsi)
    jmp .rest
    .setam:             # it always enters this bc somehow r11 is not 1 or 0
    orw $1, (%rsi)
    shll $7, (%rsi)

    # get the hr tens and ones from tod struct
    .rest:

    movq %r8, %rax # set up dividend
    cqto # extend sign
    movq $10, %rdi # set up divisor
    idiv %rdi # divide
    movq %rax, %r8 # put hour tens into r8, remainder or the hr ones is in rdx

    cmpq $0, %r8 # check if hr tens is 0 to see if i need to display it
    je .sethrones # use ax and cx
    leaq bitbask(%rip), %rax    # rax points to array, rip used to enable relocation
    movq %r8, %rdi              # rdi = hrtens, index into array
    movl (%rax,%rdi,4), %r8d    # r8 = bitbask[hrtens], note 32-bit movl and dest reg

    orl %r8d, (%rsi) # set hr tens

    .sethrones:
    shll  $7, (%rsi)

    leaq bitbask(%rip), %rax
    movq %rdx, %rdi             # rdi = hrones, index into array
    movq (%rax,%rdi,4), %rdx    # rdx = bitbask[hrones], note 32-bit movl and dest reg

    orl %edx, (%rsi) #set hr ones
    shll  $7, (%rsi)

    # get the min tens and ones from tod struct
    movq %r9, %rax # set up dividend
    cqto # extend sign
    movq $10, %rdi # set up divisor
    idivq %rdi # divide
    movq %rax, %r9 # put quotient min tens into r9, remainder or the min ones is in rdx

    leaq bitbask(%rip), %rax   # rax points to ar ray, rip used to enable relocation
    movq %r9, %rdi             # rdi = 2, index into array
    movq (%rax,%rdi,4), %r9    # r9 = bitbask[mintens], note 32-bit movl and dest reg

    orl %r9d, (%rsi) #set min tens
    shll  $7, (%rsi)

    leaq bitbask(%rip), %rax
    movq %rdx, %rdi             # rdi index into array
    movq (%rax,%rdi,4), %rdx    # rdx = bitbask[minones], note 32-bit movl and dest reg

    orl %edx, (%rsi) #set min ones

    movq $0, %rax
    ret

    .error:
    movl $0, (%rsi) # set display bits to 0
    movq $1, %rax
    ret


lcd_update:

    movl  TIME_OF_DAY_SEC(%rip), %edi  # copy global var to reg edi
   # movq $0, %rdx
    pushq $0        # align stack and make a 0 tod struct
    movq %rsp, %rsi # move pointer to a 0 struct into 2nd arg

    call set_tod_from_secs
    popq %rdi  # restore stack and put the struct into 1st arg
    cmpl $0, %eax # check for error
    jne .ERROR
    #movq $0, %r8
    pushq $0
    #movq (%rsi), %rdi
    movq %rsp, %rsi # putting 0 for the display port
    call set_display_bits_from_tod
    popq %r8            # re align stack and put it into r8
    movl %r8d, LCD_DISPLAY_PORT(%rip)  # copy reg rsi to global var

    cmpl $0, %eax # check for error
    jne .ERROR
    ret # eax will be 1 indicating a success so the function can just be returned

    .ERROR:
    ret