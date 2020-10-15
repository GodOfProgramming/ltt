    .global _main

_main:
    mov     $1, %rax      #
    mov     $1, %rdi      #
    mov     $hello, %rsi  # move the ptr of hello into rsi register
    mov     $13, %rdx
    syscall

    mov     $60, %rax
    xor     %rdi, %rdi    # zero out rax register
    syscall

hello:
    .ascii  "Hello, world\n"
