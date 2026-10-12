extern scanf, printf, strcmp, strcpy

section .data
    fmt_num db "%d", 0
    fmt_in  db "%10s", 0
    fmt_out db "%d", 10, 0

section .bss
    n       resd 1
    count   resd 1
    i       resd 1
    j       resd 1
    buf     resb 11
    strings resb 11 * 500

section .text
    global main

main:
    push ebp
    mov ebp, esp
    and esp, -16

    sub esp, 8
    push n
    push fmt_num
    call scanf
    add esp, 16

    mov dword [count], 0
    mov dword [i], 0

.cikl:
    mov eax, [i]
    cmp eax, [n]
    jge .print

    sub esp, 8
    push buf
    push fmt_in
    call scanf
    add esp, 16

    mov dword [j], 0

.sravn:
    mov eax, [j]
    cmp eax, [count]
    jge .no

    mov eax, [j]
    imul eax, 11
    lea eax, [strings + eax]

    sub esp, 8
    push buf
    push eax
    call strcmp
    add esp, 16

    test eax, eax
    jz .yes

    mov eax, [j]
    inc eax
    mov [j], eax
    jmp .sravn

.no:
    mov eax, [count]
    imul eax, 11
    lea eax, [strings + eax]

    sub esp, 8
    push buf
    push eax
    call strcpy
    add esp, 16

    inc dword [count]

.yes:
    inc dword [i]
    jmp .cikl

.print:
    sub esp, 8
    push dword [count]
    push fmt_out
    call printf
    add esp, 16

    mov esp, ebp
    pop ebp
    xor eax, eax
    ret