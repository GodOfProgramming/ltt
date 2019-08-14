	.global _start

	.data
argc:
	DQ	0

	.text
_start:
	#mov	QWORD PTR [rbp-0x10],	%rax	; also in rsi
	#add	0x8,			%rax
	#mov	QWORD PTR [rax],	%rsi
	#mov	0x40064c,		%edi	; format
	#call	400418 <printf@plt>
