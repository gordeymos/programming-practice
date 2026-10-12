#ifndef FUNCS_H
#define FUNCS_H

/* Функции варианта 10, реализованы на NASM32/x87 в asm/funcs.asm */
extern long double f1(long double x);   /* 1 + 4/(x^2+1)      */
extern long double f2(long double x);   /* x^3                 */
extern long double f3(long double x);   /* 2^(-x)              */

extern long double f1d(long double x);  /* -8x / (x^2+1)^2    */
extern long double f2d(long double x);  /* 3x^2                */
extern long double f3d(long double x);  /* -ln(2) * 2^(-x)    */

#endif
