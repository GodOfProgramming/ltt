	.file	"prime.c"
	.text
	.section	.rodata
	.align 8
.LC0:
	.string	"Usage ?prime_start=? ?prime_end=? prime [num_primes]"
.LC1:
	.string	"prime_start"
.LC2:
	.string	"prime_end"
.LC3:
	.string	"%d\n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB5:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$64, %rsp
	movl	%edi, -52(%rbp)
	movq	%rsi, -64(%rbp)
	cmpl	$1, -52(%rbp)
	jne	.L2
	leaq	.LC0(%rip), %rdi
	call	puts@PLT
	movl	$0, %eax
	jmp	.L3
.L2:
	movq	-64(%rbp), %rax
	addq	$8, %rax
	movq	(%rax), %rax
	movq	%rax, %rdi
	call	atoi@PLT
	movl	%eax, -32(%rbp)
	leaq	.LC1(%rip), %rdi
	call	getenv@PLT
	movq	%rax, -16(%rbp)
	leaq	.LC2(%rip), %rdi
	call	getenv@PLT
	movq	%rax, -8(%rbp)
	cmpq	$0, -16(%rbp)
	je	.L4
	movq	-16(%rbp), %rax
	movq	%rax, %rdi
	call	atoi@PLT
	jmp	.L5
.L4:
	movl	$1, %eax
.L5:
	movl	%eax, -28(%rbp)
	cmpq	$0, -8(%rbp)
	je	.L6
	movq	-8(%rbp), %rax
	movq	%rax, %rdi
	call	atoi@PLT
	jmp	.L7
.L6:
	movl	$2147483647, %eax
.L7:
	movl	%eax, -24(%rbp)
	movl	-28(%rbp), %eax
	movl	%eax, -48(%rbp)
	movl	$0, -44(%rbp)
	jmp	.L8
.L15:
	movl	-48(%rbp), %eax
	movl	%eax, %edx
	shrl	$31, %edx
	addl	%edx, %eax
	sarl	%eax
	movl	%eax, -20(%rbp)
	movl	$1, -40(%rbp)
	movl	$2, -36(%rbp)
	jmp	.L9
.L12:
	movl	-48(%rbp), %eax
	cltd
	idivl	-36(%rbp)
	movl	%edx, %eax
	testl	%eax, %eax
	jne	.L10
	movl	$0, -40(%rbp)
	jmp	.L11
.L10:
	addl	$1, -36(%rbp)
.L9:
	movl	-36(%rbp), %eax
	cmpl	-20(%rbp), %eax
	jle	.L12
.L11:
	cmpl	$0, -40(%rbp)
	je	.L13
	movl	-48(%rbp), %eax
	movl	%eax, %esi
	leaq	.LC3(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	addl	$1, -44(%rbp)
.L13:
	addl	$1, -48(%rbp)
.L8:
	movl	-48(%rbp), %eax
	cmpl	-24(%rbp), %eax
	jge	.L14
	movl	-44(%rbp), %eax
	cmpl	-32(%rbp), %eax
	jl	.L15
.L14:
	movl	$0, %eax
.L3:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE5:
	.size	main, .-main
	.ident	"GCC: (Ubuntu 7.4.0-1ubuntu1~18.04.1) 7.4.0"
	.section	.note.GNU-stack,"",@progbits
