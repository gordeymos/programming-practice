; funcs.asm — f1, f2, f3 и их производные на NASM32 / x87 FPU / cdecl
; Вариант 10: f1 = 1 + 4/(x^2+1), f2 = x^3, f3 = 2^(-x)
;
; Соглашение cdecl (i386 Linux):
;   long double (80 бит) занимает 12 байт на стеке (10 данных + 2 паддинг)
;   Первый аргумент: [ebp+8], результат возвращается в ST(0)

section .data
align 4
_four:  dt 4.0      ; константа 4.0 (long double)
_eight: dt 8.0      ; константа 8.0
_three: dt 3.0      ; константа 3.0

section .text
global f1, f2, f3, f1d, f2d, f3d

; ---------------------------------------------------------------
; long double f1(long double x) = 1 + 4 / (x^2 + 1)
; ---------------------------------------------------------------
f1:
    push    ebp
    mov     ebp, esp

    fldt    [ebp+8]         ; st0 = x
    fmul    st0, st0        ; st0 = x^2
    fld1                    ; st0 = 1, st1 = x^2
    faddp                   ; st0 = x^2 + 1

    ; st0 = x^2+1 — это знаменатель; нам нужно 4 / (x^2+1)
    fldt    [_four]         ; st0 = 4, st1 = x^2+1
    fxch                    ; st0 = x^2+1, st1 = 4
    fdivp                   ; st0 = 4 / (x^2+1)

    fld1                    ; st0 = 1, st1 = 4/(x^2+1)
    faddp                   ; st0 = 1 + 4/(x^2+1)

    pop     ebp
    ret

; ---------------------------------------------------------------
; long double f2(long double x) = x^3
; ---------------------------------------------------------------
f2:
    push    ebp
    mov     ebp, esp

    fldt    [ebp+8]         ; st0 = x
    fld     st0             ; st0 = x, st1 = x
    fmulp                   ; st0 = x^2
    fldt    [ebp+8]         ; st0 = x, st1 = x^2
    fmulp                   ; st0 = x^3

    pop     ebp
    ret

; ---------------------------------------------------------------
; long double f3(long double x) = 2^(-x)
;
; Алгоритм: 2^y = 2^frac(y) * 2^int(y), где y = -x
;   frac(y) = y - round(y)  (всегда в (-1, 1))
;   2^frac(y) вычисляем через f2xm1 + 1
;   2^int(y)  вычисляем через fscale
; ---------------------------------------------------------------
f3:
    push    ebp
    mov     ebp, esp

    fldt    [ebp+8]         ; st0 = x
    fchs                    ; st0 = -x  (= y)

    fld     st0             ; st0 = y,   st1 = y
    frndint                 ; st0 = round(y), st1 = y
    fsub    st1, st0        ; st1 = y - round(y) = frac(y)
    fxch                    ; st0 = frac(y), st1 = round(y)

    f2xm1                   ; st0 = 2^frac(y) - 1
    fld1                    ; st0 = 1
    faddp                   ; st0 = 2^frac(y)

    fscale                  ; st0 = 2^frac(y) * 2^round(y) = 2^y = 2^(-x)
    fstp    st(1)           ; убираем round(y), результат остаётся в st0

    pop     ebp
    ret

; ---------------------------------------------------------------
; long double f1d(long double x) = -8x / (x^2 + 1)^2
; ---------------------------------------------------------------
f1d:
    push    ebp
    mov     ebp, esp

    ; Вычислим (x^2+1)^2
    fldt    [ebp+8]         ; st0 = x
    fmul    st0, st0        ; st0 = x^2
    fld1                    ; st0 = 1, st1 = x^2
    faddp                   ; st0 = x^2 + 1
    fld     st0             ; st0 = x^2+1, st1 = x^2+1
    fmulp                   ; st0 = (x^2+1)^2

    ; Вычислим -8x
    fldt    [_eight]        ; st0 = 8, st1 = (x^2+1)^2
    fldt    [ebp+8]         ; st0 = x, st1 = 8, st2 = (x^2+1)^2
    fmulp                   ; st0 = 8x, st1 = (x^2+1)^2
    fchs                    ; st0 = -8x, st1 = (x^2+1)^2

    ; -8x / (x^2+1)^2
    fdivrp                  ; st0 = (-8x) / (x^2+1)^2

    pop     ebp
    ret

; ---------------------------------------------------------------
; long double f2d(long double x) = 3 * x^2
; ---------------------------------------------------------------
f2d:
    push    ebp
    mov     ebp, esp

    fldt    [ebp+8]         ; st0 = x
    fmul    st0, st0        ; st0 = x^2
    fldt    [_three]        ; st0 = 3, st1 = x^2
    fmulp                   ; st0 = 3 * x^2

    pop     ebp
    ret

; ---------------------------------------------------------------
; long double f3d(long double x) = -ln(2) * 2^(-x)
; ---------------------------------------------------------------
f3d:
    push    ebp
    mov     ebp, esp

    ; Вычислим 2^(-x) — то же что и в f3
    fldt    [ebp+8]         ; st0 = x
    fchs                    ; st0 = -x = y

    fld     st0             ; st0 = y, st1 = y
    frndint                 ; st0 = round(y), st1 = y
    fsub    st1, st0        ; st1 = frac(y)
    fxch                    ; st0 = frac(y), st1 = round(y)

    f2xm1                   ; st0 = 2^frac(y) - 1
    fld1
    faddp                   ; st0 = 2^frac(y)
    fscale                  ; st0 = 2^(-x)
    fstp    st(1)           ; убираем round(y)

    ; Умножим на ln(2) и смените знак
    fldln2                  ; st0 = ln(2), st1 = 2^(-x)
    fmulp                   ; st0 = ln(2) * 2^(-x)
    fchs                    ; st0 = -ln(2) * 2^(-x)

    pop     ebp
    ret
