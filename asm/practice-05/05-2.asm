extern scanf, printf, strstr

section .data
    fmt_in  db "%1000s", 0
    fmt_21  db "2 1", 10, 0
    fmt_12  db "1 2", 10, 0
    fmt_0   db "0", 10, 0

section .bss
    str1 resb 1001
    str2 resb 1001

section .text
    global main

main:
    push ebp
    mov ebp, esp
    and esp, -16
    sub esp, 8
    push str1
    push fmt_in
    call scanf
    add esp, 16
    sub esp, 8
    push str2
    push fmt_in
    call scanf
    add esp, 16
    sub esp, 8
    push str2
    push str1
    call strstr
    add esp, 16
    test eax, eax
    jnz .found_21
    sub esp, 8
    push str1
    push str2
    call strstr
    add esp, 16
    test eax, eax
    mov eax, fmt_0
    jz .print
    mov eax, fmt_12
    jmp .print

.found_21:
    mov eax, fmt_21

.print:
    sub esp, 12
    push eax
    call printf
    add esp, 16
    mov esp, ebp
    pop ebp
    xor eax, eax
    ret