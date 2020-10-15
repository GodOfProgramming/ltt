	.global _start

	.text
_start:
# char* buff = malloc(32);
	mov			$32, %rdi
	call		malloc
	mov 		%rax, %r8

# size_t output_len = strlen(prompt);
	mov 		$prompt, %rdi
	call		strlen
	mov			$rax, %r15

# write(1, prompt, output_len);
	mov     $1, %rax      							# 1 is the syscall for write
	mov     $1, %rdi      							# 1 is the file descriptor for stdout
	mov     $prompt, %rsi  							# move the ptr of hello into rsi register
	mov     $r15, %rdx									# output_len
	syscall
	
# read(0, buff, 31); // 31 = sizeof(buff) - 1
	mov			$0, %rax 										# 0 syscall for read
	mov			$0, %rdi										# 0 is file desc for stdin
	mov			%r8, %rsi										# ptr to buffer
	mov			$31, %rdx										# length of buffer - 1
	syscall

# output_len = strlen(display_msg);
	mov 		$display_msg, %rdi
	call		strlen
	mov			$rax, %r15

# write(1, display_msg, output_len);
	mov     $1, %rax      							# 1 is the syscall for write
	mov     $1, %rdi      							# 1 is the file descriptor for stdout
	mov     $display_msg, %rsi					# move the ptr of hello into rsi register
	mov     $r15, %rdx									# promtp_len
	syscall

# write(1, buff, 32);
	mov			$1, %rax
	mov			$1, %rdi
	mov			$r8, %rsi
	mov			$32, %rdx
	syscall

# putchar('\n');
	mov 		$10, %rdi										# '\n' as only arg
	call 		putchar
	
	# exit(0)
	mov     $60, %rax
	xor     %rdi, %rdi    							# zero out rax register
	syscall

prompt:
	.asciz "Enter a string: "

display_msg:
	.asciz  "You said: "
