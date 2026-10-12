%include "io.inc"

section .bss
    N resd 1
    K resd 1

section .text
global main

sum:
    push ebp
    mov ebp, esp
    push ebx
    mov eax, [ebp+8]
    mov ecx, [ebp+12]
    xor ebx, ebx
.loop:
    test eax, eax
    jz .done
    xor edx, edx
    div ecx
    add ebx, edx
    jmp .loop
.done:
    mov eax, ebx
    pop ebx
    pop ebp
    ret

main:
    push ebx
    push edi
    push esi

    GET_DEC 4, N
    GET_DEC 4, K
    mov edi, [N]
    mov ebx, [K]
    xor esi, esi
.loop:
    add esi, edi
    push ebx
    push edi
    call sum
    add esp, 8
    cmp eax, edi
    je .exit
    mov edi, eax
    jmp .loop
.exit:
    add esi, eax
    PRINT_DEC 4, esi
    NEWLINE

    pop esi
    pop edi
    pop ebx
    xor eax, eax
    ret