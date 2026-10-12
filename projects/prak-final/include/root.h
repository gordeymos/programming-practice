#ifndef ROOT_H
#define ROOT_H

/*
 * root — нахождение x: f(x) = g(x) на отрезке [a, b] с точностью eps.
 *
 * Метод выбирается препроцессором:
 *   -DMETHOD_NEWTON    — метод Ньютона (касательных)
 *   -DMETHOD_COMBINED  — комбинированный (хорды + касательные)
 *
 * Параметры:
 *   f, g   — функции (f(x) = g(x) ищем)
 *   df, dg — их производные
 *   a, b   — концы отрезка, на котором ровно один корень
 *   eps    — требуемая точность
 *
 * Глобальная переменная root_iters после вызова содержит число итераций.
 */
long double root(
    long double (*f)(long double),
    long double (*g)(long double),
    long double (*df)(long double),
    long double (*dg)(long double),
    long double a,
    long double b,
    long double eps
);

extern int root_iters;

#endif
