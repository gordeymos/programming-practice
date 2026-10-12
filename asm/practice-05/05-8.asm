extern fopen, fclose, fscanf, fprintf, qsort

section .data
    mode_r  db "r", 0
    mode_w  db "w", 0
    file_in db "input.txt", 0
    file_out db "output.txt", 0
    fmt_in  db "%d", 0
    fmt_out db "%d ", 0

section .bss
    fin     resd 1
    fout    resd 1
    arr     resd 1001
    count   resd 1
    tmp     resd 1

section .text
    global main

sravn:
    push ebp
    mov ebp, esp
    mov eax, [ebp+8]
    mov ecx, [ebp+12]
    mov eax, [eax]
    mov ecx, [ecx]
    cmp eax, ecx
    jl .less
    jg .greater
    xor eax, eax
    jmp .done
.less:
    mov eax, -1
    jmp .done
.greater:
    mov eax, 1
.done:
    pop ebp
    ret

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

    sub esp, 8
    push mode_w
    push file_out
    call fopen
    add esp, 16
    mov [fout], eax

    mov dword [count], 0

.cikl:
    mov eax, [count]
    imul eax, 4
    lea eax, [arr + eax]

    sub esp, 4
    push eax
    push fmt_in
    push dword [fin]
    call fscanf
    add esp, 16

    cmp eax, 1
    jne .sort

    inc dword [count]
    jmp .cikl

.sort:
    sub esp, 4
    push sravn
    push dword 4
    push dword [count]
    push arr
    call qsort
    add esp, 16

    mov dword [tmp], 0

.zap:
    mov eax, [tmp]
    cmp eax, [count]
    jge .end

    mov eax, [tmp]
    imul eax, 4
    mov eax, [arr + eax]

    sub esp, 8
    push eax
    push fmt_out
    push dword [fout]
    call fprintf
    add esp, 16

    inc dword [tmp]
    jmp .zap

.end:
    sub esp, 12
    push dword [fin]
    call fclose
    add esp, 16

    sub esp, 12
    push dword [fout]
    call fclose
    add esp, 16

    mov esp, ebp
    pop ebp
    xor eax, eax
    ret