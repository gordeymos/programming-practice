extern scanf, printf

section .data
    fmt_in  db "%u", 0
    fmt_out db "0x%08X", 10, 0

section .bss
    num resd 1

section .text
    global main

main:
    push ebp
    mov ebp, esp
    and esp, -16

.func:
    sub esp, 8
    push num
    push fmt_in
    call scanf
    add esp, 16
    cmp eax, 1
    jne .finish
    sub esp, 8
    push dword [num]
    push fmt_out
    call printf
    add esp, 16
    jmp .func

.finish:
    mov esp, ebp
    pop ebp
    xor eax, eax
    ret