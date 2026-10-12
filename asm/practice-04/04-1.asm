%include "io.inc"

section .bss
    a resd 1
    b resd 1
    c resd 1
    d resd 1

section .text
global main

gcd:
    push ebp
    mov ebp, esp
    push ebx
    mov eax, [ebp+8]
    mov ebx, [ebp+12]
.loop:
    test ebx, ebx
    jz .finish
    xor edx, edx
    div ebx
    mov eax, ebx
       mov ebx, edx
    jmp .loop
.finish:
    pop ebx
    pop ebp
    ret

main:
    GET_DEC 4, a
    GET_DEC 4, b
    GET_DEC 4, c
    GET_DEC 4, d
    push dword [b]
    push dword [a]
    call gcd
    add esp, 8
    push dword [c]
    push eax
    call gcd
    add esp, 8
    push dword [d]
    push eax
    call gcd
    add esp, 8
    PRINT_DEC 4, eax
    NEWLINE
    xor eax, eax
    ret