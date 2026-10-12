%include "io.inc"

section .bss
    N resd 1
    k resd 1
    arr resd 1000

section .text
    global main

count_zeros:
    push ebp
    mov ebp, esp
    push ecx
    mov eax, [ebp+8]
    xor ecx, ecx
.loop:
    test eax, eax
    jz .done
    test al, 1
    jnz .skip
    inc ecx
.skip:
    shr eax, 1
    jmp .loop
.done:
    mov eax, ecx
    pop ecx
    pop ebp
    ret

main:
    push ebx
    push edi
    GET_DEC 4, N
    mov ecx, [N]
    mov edi, arr
.read_loop:
    test ecx, ecx
    jz .read_done
    push ecx
    GET_DEC 4, [edi]
    pop ecx
    add edi, 4
    dec ecx
    jmp .read_loop
.read_done:
    GET_DEC 4, k
    mov ecx, [N]
    mov edi, arr
    xor ebx, ebx
.check_loop:
    test ecx, ecx
    jz .check_done
    push dword [edi]
    call count_zeros
    add esp, 4
    cmp eax, [k]
    jne .skip
    inc ebx
.skip:
    add edi, 4
    dec ecx
    jmp .check_loop
.check_done:
    PRINT_DEC 4, ebx
    NEWLINE
    pop edi
    pop ebx
    xor eax, eax
    ret