	.file	"lcd_update.c"
	.text
	.globl	set_tod_from_secs
	.type	set_tod_from_secs, @function
set_tod_from_secs:
.LFB38:
	.cfi_startproc
	leal	-1(%rdi), %eax
	cmpl	$86398, %eax
	ja	.L2
	movl	$-1851608123, %edx
	movl	%edi, %eax
	imull	%edx
	leal	(%rdx,%rdi), %ecx
	sarl	$11, %ecx
	movl	%edi, %eax
	sarl	$31, %eax
	subl	%eax, %ecx
	movw	%cx, (%rsi)
	movswl	%cx, %r8d
	imull	$-3600, %r8d, %r8d
	addl	%edi, %r8d
	movl	$-2004318071, %edx
	movl	%r8d, %eax
	imull	%edx
	addl	%r8d, %edx
	sarl	$5, %edx
	sarl	$31, %r8d
	subl	%r8d, %edx
	movw	%dx, 2(%rsi)
	imulw	$3600, %cx, %ax
	leal	0(,%rdx,4), %r8d
	sall	$6, %edx
	subl	%r8d, %edx
	addl	%eax, %edx
	subl	%edx, %edi
	movw	%di, 4(%rsi)
	cmpw	$12, %cx
	jle	.L3
	subl	$12, %ecx
	movw	%cx, (%rsi)
	movb	$1, 6(%rsi)
	movl	$0, %eax
	ret
.L3:
	cmpw	$12, %cx
	jne	.L5
	movb	$1, 6(%rsi)
	movl	$0, %eax
	ret
.L5:
	testw	%cx, %cx
	jne	.L6
	movw	$12, (%rsi)
	movb	$0, 6(%rsi)
	movl	$0, %eax
	ret
.L6:
	movb	$0, 6(%rsi)
	movl	$0, %eax
	ret
.L2:
	testl	%edi, %edi
	jne	.L7
	movw	$12, (%rsi)
	movb	$0, 6(%rsi)
	movw	$0, 4(%rsi)
	movw	$0, 2(%rsi)
	movl	$0, %eax
	ret
.L7:
	movl	$1, %eax
	ret
	.cfi_endproc
.LFE38:
	.size	set_tod_from_secs, .-set_tod_from_secs
	.globl	set_display_bits_from_tod
	.type	set_display_bits_from_tod, @function
set_display_bits_from_tod:
.LFB39:
	.cfi_startproc
	subq	$72, %rsp
	.cfi_def_cfa_offset 80
	movq	%fs:40, %rax
	movq	%rax, 56(%rsp)
	xorl	%eax, %eax
	cmpw	$12, %di
	ja	.L9
	movl	%edi, %eax
	sarl	$16, %eax
	testw	%ax, %ax
	js	.L9
	cmpw	$59, %ax
	jg	.L9
	movq	%rdi, %rdx
	salq	$16, %rdx
	sarq	$48, %rdx
	testw	%dx, %dx
	js	.L9
	cmpw	$59, %dx
	jg	.L9
	movl	$63, (%rsp)
	movl	$6, 4(%rsp)
	movl	$91, 8(%rsp)
	movl	$79, 12(%rsp)
	movl	$102, 16(%rsp)
	movl	$109, 20(%rsp)
	movl	$125, 24(%rsp)
	movl	$7, 28(%rsp)
	movl	$127, 32(%rsp)
	movl	$111, 36(%rsp)
	movl	$0, 40(%rsp)
	movl	$0, (%rsi)
	imull	$26215, %eax, %edx
	sarl	$18, %edx
	movl	%eax, %ecx
	sarw	$15, %cx
	subl	%ecx, %edx
	movl	%edx, %ecx
	leal	(%rdx,%rdx,4), %edx
	addl	%edx, %edx
	subl	%edx, %eax
	movswl	%ax, %edx
	movswl	%cx, %ecx
	movswl	%di, %eax
	imull	$26215, %eax, %eax
	sarl	$18, %eax
	movl	%edi, %r8d
	sarw	$15, %r8w
	subl	%r8d, %eax
	movl	%eax, %r8d
	leal	(%rax,%rax,4), %eax
	addl	%eax, %eax
	movl	%edi, %r9d
	subl	%eax, %r9d
	movl	%r9d, %eax
	cwtl
	movswl	%r8w, %r8d
	salq	$8, %rdi
	sarq	$56, %rdi
	testb	%dil, %dil
	jne	.L10
	movl	$128, (%rsi)
	jmp	.L11
.L10:
	movl	$256, (%rsi)
.L11:
	testl	%r8d, %r8d
	je	.L12
	movslq	%r8d, %r8
	movl	(%rsp,%r8,4), %edi
	orl	%edi, (%rsi)
.L12:
	movl	(%rsi), %edi
	sall	$7, %edi
	cltq
	orl	(%rsp,%rax,4), %edi
	movl	%edi, %eax
	sall	$7, %eax
	movslq	%ecx, %rcx
	orl	(%rsp,%rcx,4), %eax
	sall	$7, %eax
	movslq	%edx, %rdx
	orl	(%rsp,%rdx,4), %eax
	movl	%eax, (%rsi)
	movl	$0, %eax
	jmp	.L13
.L9:
	movl	$0, (%rsi)
	movl	$1, %eax
.L13:
	movq	56(%rsp), %rcx
	xorq	%fs:40, %rcx
	je	.L14
	call	__stack_chk_fail
.L14:
	addq	$72, %rsp
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE39:
	.size	set_display_bits_from_tod, .-set_display_bits_from_tod
	.globl	lcd_update
	.type	lcd_update, @function
lcd_update:
.LFB40:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushq	%rbx
	.cfi_def_cfa_offset 24
	.cfi_offset 3, -24
	subq	$8, %rsp
	.cfi_def_cfa_offset 32
	movl	$8, %edi
	call	malloc
	movq	%rax, %rbx
	movq	%rax, %rsi
	movl	TIME_OF_DAY_SEC(%rip), %edi
	call	set_tod_from_secs
	testl	%eax, %eax
	jne	.L17
	movl	$LCD_DISPLAY_PORT, %esi
	movq	(%rbx), %rdi
	call	set_display_bits_from_tod
	movl	%eax, %ebp
	testl	%eax, %eax
	jne	.L18
	movq	%rbx, %rdi
	call	free
	jmp	.L19
.L18:
	movq	%rbx, %rdi
	call	free
	movl	$1, %ebp
	jmp	.L19
.L17:
	movq	%rbx, %rdi
	call	free
	movl	$1, %ebp
.L19:
	movl	%ebp, %eax
	addq	$8, %rsp
	.cfi_def_cfa_offset 24
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE40:
	.size	lcd_update, .-lcd_update
	.ident	"GCC: (Ubuntu 5.4.0-6ubuntu1~16.04.9) 5.4.0 20160609"
	.section	.note.GNU-stack,"",@progbits
