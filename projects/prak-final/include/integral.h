#ifndef INTEGRAL_H
#define INTEGRAL_H

/*
 * integral — вычисление определённого интеграла ∫[a,b] f(x)dx
 * методом Симпсона с контролем погрешности по правилу Рунге.
 *
 * Начальное число разбиений n=4, удвоение до |I(2n)-I(n)|/15 < eps.
 *
 * Глобальная переменная integral_iters после вызова содержит
 * итоговое число разбиений n.
 */
long double integral(
    long double (*f)(long double),
    long double a,
    long double b,
    long double eps
);

extern int integral_iters;

#endif
