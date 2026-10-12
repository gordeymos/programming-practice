%include "io.inc"

section .bss
    N       resd 1
    num     resd 1
    den     resd 1
    sum_num resd 1
    sum_den resd 1
    counter resd 1

section .text
    global main

find_gcd:
    push ebp
    mov ebp, esp
    push ebx
    mov eax, [ebp+8]
    mov ebx, [ebp+12]
.loop:
    test ebx, ebx
    jz .done
    xor edx, edx
    div ebx
    mov eax, ebx
    mov ebx, edx
    jmp .loop
.done:
    pop ebx
    pop ebp
    ret

main:
    push ebp
    mov ebp, esp
    push ebx
    push edi
    push esi

    GET_DEC 4, N

    GET_DEC 4, sum_num
    GET_DEC 4, sum_den

    mov eax, [N]
    dec eax
    mov [counter], eax
    jz .print_result

.add_f:
    GET_DEC 4, num
    GET_DEC 4, den

    mov eax, [sum_num]
    imul eax, [den]
    mov edi, eax
    mov eax, [num]
    imul eax, [sum_den]
    add eax, edi
    mov [num], eax

    mov eax, [sum_den]
    imul eax, [den]
    mov [den], eax

    push dword [den]
    push dword [num]
    call find_gcd
    add esp, 8

    mov ebx, eax
    mov eax, [num]
    xor edx, edx
    div ebx
    mov [sum_num], eax

    mov eax, [den]
    xor edx, edx
    div ebx
    mov [sum_den], eax

    mov eax, [counter]
    dec eax
    mov [counter], eax
    jnz .add_f

.print_result:
    PRINT_DEC 4, sum_num
    PRINT_CHAR ' '
    PRINT_DEC 4, sum_den
    NEWLINE

    pop esi
    pop edi
    pop ebx
    mov esp, ebp
    pop ebp
    xor eax, eax
    ret