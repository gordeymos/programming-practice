section .note.GNU-stack noalloc noexec nowrite progbits

extern fopen, fclose, fscanf, fprintf, malloc, free

section .data
    file_in db "input.txt", 0
    mode_r  db "r", 0
    n_in  db "%d", 0
    file_out db "output.txt", 0
    mode_w   db "w", 0
    m_out  db "%d ", 0

section .bss
    fin resd 1
    n resd 1
    head resd 1
    tail resd 1
    cur resd 1
    swapped resd 1
    m resd 1

section .text
    global main

main:
    push ebp
    mov ebp, esp
    and esp, -16

    sub esp, 8
    push mode_r
    push file_in
    call fopen
    add esp, 16
    mov [fin], eax

.cikl:
    sub esp, 4
    push n
    push n_in
    push dword [fin]
    call fscanf
    add esp, 16

    cmp eax, 1
    jne .sort

    sub esp, 12
    push 8
    call malloc
    add esp, 16
    mov dword [eax+0], 0
    mov ecx, [n]
    mov dword [eax+4], ecx

    cmp dword [head], 0
    je .spisok_pust

    mov ecx, [tail]
    mov [ecx], eax
    mov [tail], eax

    jmp .cikl

.spisok_pust:
    mov [head], eax
    mov [tail], eax
    jmp .cikl

.sort:
    mov dword [swapped], 0
    mov eax, [head]
    mov [cur], eax

.buble_sort:
    mov eax, [cur]
    mov ecx, [eax]
    cmp ecx, 0
    je .check_swap

    mov eax, [cur]
    mov eax, [eax+4]
    mov edx, [ecx+4]

    cmp eax, edx
    jle .no_swap

    mov dword [swapped], 1
    mov ebx, [cur]
    mov [ebx+4], edx
    mov [ecx+4], eax

.no_swap:
    mov eax, [cur]
    mov eax, [eax]
    mov [cur], eax
    jmp .buble_sort

.check_swap:
    cmp dword [swapped], 0
    jne .sort

    sub esp, 8
    push mode_w
    push file_out
    call fopen
    add esp, 16
    mov [m], eax

    mov eax, [head]
    mov [cur], eax

.cikl_out:
    cmp dword [cur], 0
    je .finish

    mov eax, [cur]
    sub esp, 4
    push dword [eax+4]
    push m_out
    push dword [m]
    call fprintf
    add esp, 16

    mov eax, [cur]
    mov eax, [eax]
    mov [cur], eax
    jmp .cikl_out

.finish:
    mov esp, ebp
    pop ebp
    xor eax, eax
    ret