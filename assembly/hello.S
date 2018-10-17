    .global _start

    .text
_start:
    mov     $1, %rax      #
    mov     $1, %rdi      #
    mov     $hello, %rsi  # move the ptr of hello into rsi register
    mov     $13, %rdx
    syscall

    mov     $60, %rax
    xor     %rdi, %rdi    # zero out rax register
    syscall

    call    exit          # call exit proc

exit:                     # jmp here to exit process
    mov     $1, %rax      # move sys_exit code into syscall reg
    mov     $0, %rbx      # move ret_val into ebx
    syscall

amem:                     # allocate memory


hello:
    .ascii  "Hello, world\n"
