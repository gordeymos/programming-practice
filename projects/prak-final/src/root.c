#include "root.h"
#include <math.h>

int root_iters = 0;

/* Вспомогательная функция: числовая оценка F''(x) */
static long double d2F(
    long double (*f)(long double),
    long double (*g)(long double),
    long double x)
{
    long double h = 1e-5L;
    long double Fx  = f(x)   - g(x);
    long double Fxh = f(x+h) - g(x+h);
    long double Fxn = f(x-h) - g(x-h);
    return (Fxh - 2.0L*Fx + Fxn) / (h * h);
}

/* ============================================================
 * Метод Ньютона (касательных)
 *
 * F(x) = f(x) - g(x),  F'(x) = df(x) - dg(x)
 * x_{n+1} = x_n - F(x_n) / F'(x_n)
 *
 * Начальное приближение x0: конец отрезка, где F(x0)*F''(x0) > 0
 * (условие гарантированной сходимости метода Ньютона).
 * ============================================================ */
#ifdef METHOD_NEWTON

long double root(
    long double (*f)(long double),
    long double (*g)(long double),
    long double (*df)(long double),
    long double (*dg)(long double),
    long double a,
    long double b,
    long double eps)
{
    long double Fa = f(a) - g(a);
    long double Fb = f(b) - g(b);

    /* Выбираем начальную точку x0, где F*F'' > 0 */
    long double x;
    if (Fa * d2F(f, g, a) > 0.0L)
        x = a;
    else if (Fb * d2F(f, g, b) > 0.0L)
        x = b;
    else
        x = (a + b) / 2.0L;   /* запасной вариант */

    root_iters = 0;
    while (1) {
        long double Fx  = f(x) - g(x);
        long double dFx = df(x) - dg(x);
        long double step = Fx / dFx;
        x -= step;
        root_iters++;
        if (fabsl(step) < eps)
            break;
        if (root_iters > 10000)
            break;
    }
    return x;
}

#endif /* METHOD_NEWTON */

/* ============================================================
 * Комбинированный метод (хорды + касательные)
 *
 * Из двух концов отрезка:
 *   — со стороны, где F*F'' > 0, идём методом касательных (Ньютон)
 *   — с другой стороны — методом хорд
 *
 * Оба конца сходятся к корню с разных сторон.
 * Останавливаемся, когда |x_newton - x_chord| < eps.
 * ============================================================ */
#ifdef METHOD_COMBINED

long double root(
    long double (*f)(long double),
    long double (*g)(long double),
    long double (*df)(long double),
    long double (*dg)(long double),
    long double a,
    long double b,
    long double eps)
{
    long double Fa = f(a) - g(a);

    /* Определяем, какой конец — «ньютоновский» */
    long double xn, xc;
    if (Fa * d2F(f, g, a) > 0.0L) {
        xn = a;   /* Newton  — слева */
        xc = b;   /* Chord   — справа */
    } else {
        xn = b;   /* Newton  — справа */
        xc = a;   /* Chord   — слева */
    }

    root_iters = 0;
    while (fabsl(xn - xc) >= eps) {
        /* Шаг Ньютона */
        long double Fxn  = f(xn) - g(xn);
        long double dFxn = df(xn) - dg(xn);
        xn = xn - Fxn / dFxn;

        /* Шаг хорды */
        long double Fxc  = f(xc) - g(xc);
        long double Fxn2 = f(xn) - g(xn);
        xc = xc - Fxc * (xn - xc) / (Fxn2 - Fxc);

        root_iters++;
        if (root_iters > 10000)
            break;
    }
    return (xn + xc) / 2.0L;
}

#endif /* METHOD_COMBINED */
