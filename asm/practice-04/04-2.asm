%include "io.inc"

section .text
global main

rev:
    push ebp
    mov ebp, esp
    sub esp, 4
    GET_DEC 4, [ebp-4]
    mov eax, [ebp-4]
    test eax, eax
    jz .end
    call rev
    test eax, eax
    jz .nospace
    PRINT_CHAR ' '
.nospace:
    PRINT_DEC 4, [ebp-4]
    mov eax, 1
    mov esp, ebp
    pop ebp
    ret
.end:
    xor eax, eax
    mov esp, ebp
    pop ebp
    ret

main:
    call rev
    test eax, eax
    jz .skip
    NEWLINE
.skip:
    xor eax, eax
    ret