%include "io.inc"

section .bss
    K    resd 1
    N    resd 1
    A    resd 1

section .text
    global main

num:
    push ebp
    mov ebp, esp
    push ebx
    mov eax, [ebp+8]
    mov ebx, [ebp+12]
    mov ecx, 1
.loop:
    xor edx, edx
    div ebx
    test eax, eax
    jz .done
    inc ecx
    jmp .loop
.done:
    mov eax, ecx
    pop ebx
    pop ebp
    ret

step:
    push ebp
    mov ebp, esp
    push ebx
    mov ebx, [ebp+8]
    mov ecx, [ebp+12]
    mov eax, 1
.loop:
    test ecx, ecx
    jz .done
    imul eax, ebx
    dec ecx
    jmp .loop
.done:
    pop ebx
    pop ebp
    ret

append:
    push ebp
    mov ebp, esp
    push ebx
    push esi
    push edi

    push dword [ebp+16]
    push dword [ebp+12]
    call num
    add esp, 8
    mov esi, eax

    push esi
    push dword [ebp+16]
    call step
    add esp, 8
    mov edi, eax

    mov eax, [ebp+8]
    imul eax, edi
    add eax, [ebp+12]
    xor edx, edx
    mov ebx, 2011
    div ebx
    mov eax, edx

    pop edi
    pop esi
    pop ebx
    pop ebp
    ret

main:
    push ebp
    mov ebp, esp
    push ebx
    push edi
    push esi

    GET_DEC 4, K
    GET_DEC 4, N
    GET_DEC 4, A

    mov eax, [A]
    xor edx, edx
    mov ebx, 2011
    div ebx
    mov esi, edx

    push dword [K]
    push esi
    push esi
    call append
    add esp, 12
    mov edi, eax

    mov eax, [N]
    cmp eax, 1
    je .print_result

    dec eax
    mov ebx, eax

.compute_next:
    push dword [K]
    push esi
    push edi
    call append
    add esp, 12

    mov esi, edi
    mov edi, eax

    dec ebx
    jnz .compute_next

.print_result:
    PRINT_DEC 4, edi
    NEWLINE

    pop esi
    pop edi
    pop ebx
    mov esp, ebp
    pop ebp
    xor eax, eax
    ret