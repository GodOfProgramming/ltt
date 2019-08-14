	.file	"test.c"
	.text
	.globl	foo
	.data
	.align 4
	.type	foo, @object
	.size	foo, 4
foo:
	.long	6
	.text
	.globl	ul
	.type	ul, @function
ul:
.LFB5:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	%edi, -4(%rbp)
	movl	%esi, -8(%rbp)
	movl	%edx, -12(%rbp)
	movl	%ecx, -16(%rbp)
	movl	-8(%rbp), %eax
	cmpl	%eax, -4(%rbp)
	cmovle	-4(%rbp), %eax
	movl	-12(%rbp), %edx
	subl	%eax, %edx
	movl	%edx, %eax
	cltd
	xorl	%edx, %eax
	subl	%edx, %eax
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE5:
	.size	ul, .-ul
	.globl	br
	.type	br, @function
br:
.LFB6:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	%edi, -4(%rbp)
	movl	%esi, -8(%rbp)
	movl	%edx, -12(%rbp)
	movl	%ecx, -16(%rbp)
	movl	-8(%rbp), %eax
	cmpl	%eax, -4(%rbp)
	cmovge	-4(%rbp), %eax
	subl	-12(%rbp), %eax
	addl	$2, %eax
	cltd
	xorl	%edx, %eax
	subl	%edx, %eax
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE6:
	.size	br, .-br
	.section	.rodata
.LC0:
	.string	"%d"
.LC1:
	.string	"%d "
	.global	_start
	.text
_start:
.LFB7:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	pushq	%rbx
	subq	$40, %rsp
	.cfi_offset 3, -24
	movq	%fs:40, %rax
	movq	%rax, -24(%rbp)
	xorl	%eax, %eax
	leaq	-48(%rbp), %rax
	movq	%rax, %rsi
	leaq	.LC0(%rip), %rdi
	movl	$0, %eax
	call	__isoc99_scanf@PLT
	movl	-48(%rbp), %eax
	addl	%eax, %eax
	subl	$1, %eax
	movl	%eax, -40(%rbp)
	movl	$0, -44(%rbp)
	jmp	.L6
.L10:
	cmpl	$0, -44(%rbp)
	jle	.L7
	movl	-44(%rbp), %eax
	cltd
	idivl	-40(%rbp)
	movl	%edx, %eax
	testl	%eax, %eax
	jne	.L7
	movl	$10, %edi
	call	putchar@PLT
.L7:
	movl	-44(%rbp), %eax
	cltd
	idivl	-40(%rbp)
	movl	%eax, -36(%rbp)
	movl	-44(%rbp), %eax
	cltd
	idivl	-40(%rbp)
	movl	%edx, -32(%rbp)
	movl	-48(%rbp), %edx
	movl	-40(%rbp), %ecx
	movl	-32(%rbp), %esi
	movl	-36(%rbp), %eax
	movl	%eax, %edi
	call	ul
	movl	%eax, %ebx
	movl	-48(%rbp), %edx
	movl	-40(%rbp), %ecx
	movl	-32(%rbp), %esi
	movl	-36(%rbp), %eax
	movl	%eax, %edi
	call	br
	cmpl	%eax, %ebx
	jge	.L8
	movl	-48(%rbp), %edx
	movl	-40(%rbp), %ecx
	movl	-32(%rbp), %esi
	movl	-36(%rbp), %eax
	movl	%eax, %edi
	call	br
	jmp	.L9
.L8:
	movl	-48(%rbp), %edx
	movl	-40(%rbp), %ecx
	movl	-32(%rbp), %esi
	movl	-36(%rbp), %eax
	movl	%eax, %edi
	call	ul
.L9:
	movl	%eax, -28(%rbp)
	movl	-28(%rbp), %eax
	movl	%eax, %esi
	leaq	.LC1(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	addl	$1, -44(%rbp)
.L6:
	movl	-40(%rbp), %eax
	imull	-40(%rbp), %eax
	cmpl	%eax, -44(%rbp)
	jl	.L10
	movl	$0, %eax
	movq	-24(%rbp), %rbx
	xorq	%fs:40, %rbx
	je	.L12
	call	__stack_chk_fail@PLT
.L12:
	addq	$40, %rsp
	popq	%rbx
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE7:
	.size	_start, .-_start
	.ident	"GCC: (Ubuntu 7.4.0-1ubuntu1~18.04.1) 7.4.0"
	.section	.note.GNU-stack,"",@progbits
