; Data section for vars
section		.data

EXIT_STATUS	equ	0	; program exit code
SYS_exit	equ	60	; syscall exit code

byte1		db	0 	; some byte
byte2		db	0	; another one
res		db	0	; result

; Text section for code
section		.text
global _start
_start:

; example, adding two bytes
	;mov	al, [byte1]
	;add	al, [byte2]
	;move	[res], al

; done, end program
	mov	rax, SYS_exit
	mov	rdi, EXIT_STATUS
	syscall

