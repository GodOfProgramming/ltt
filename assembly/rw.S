	.section .data

INPUT_LEN: # const size_t
	.long 	32

buff: # char*
	.long 	0

prompt: # const char*
	.asciz	"Enter a string: "

input_fmt: # const char*
	.asciz	"%s"

display_fmt: # const char*
	.asciz  "You said: %s\n"

	.section .text

	.global main

main:
# buff = malloc(INPUT_LEN);
	push 		%rbx
	mov			INPUT_LEN(%rip), %rdi
	call		malloc
	mov 		%rax, buff(%rip)
	pop			%rbx

# bzero(buff, INPUT_LEN);
	push		%rbx
	mov			buff(%rip), %rdi
	mov			INPUT_LEN(%rip), %rsi
	call		bzero
	pop			%rbx

# printf(prompt);
	push		%rbx
	lea 		prompt(%rip), %rdi
	xor			%rax, %rax										# clear rax so printf expects no vector args
	call		printf
	pop			%rbx

# scanf(input_fmt, &buff);
	push		%rbx
	lea			input_fmt(%rip), %rdi
	lea			buff(%rip), %rsi
	xor			%rax, %rax
	call		scanf
	pop			%rbx

# printf(dislay_fmt, buff);
	push		%rbx
	lea 		display_fmt(%rip), %rdi
	lea			buff(%rip), %rsi
	xor			%rax, %rax										# clear rax so printf expects no vector args
	call		printf
	pop			%rbx

# free(buff);
	push		%rbx
	lea			buff(%rip), %rdi
	call		free
	pop			%rbx

# return 0
	mov			$0, %rax
	ret

