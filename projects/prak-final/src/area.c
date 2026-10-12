#include "area.h"
#include "funcs.h"
#include "root.h"
#include "integral.h"

/*
 * Геометрия фигуры (вариант 10):
 *
 *   f1(x) = 1 + 4/(x^2+1)  — верхняя кривая на всём диапазоне фигуры
 *   f2(x) = x^3
 *   f3(x) = 2^(-x)
 *
 * Три точки пересечения:
 *   x13 — f1 = f3,  интервал поиска [-2, -1]
 *   x23 — f2 = f3,  интервал поиска [ 0,  1]
 *   x12 — f1 = f2,  интервал поиска [ 1,  2]
 *
 * Площадь фигуры:
 *   S = ∫[x13, x23] (f1(x) - f3(x)) dx
 *     + ∫[x23, x12] (f1(x) - f2(x)) dx
 *
 * Обоснование: в [x13, x23] нижняя граница — f3 (f3 > f2 в этом диапазоне),
 *              в [x23, x12] нижняя граница — f2 (f2 > f3 после точки x23).
 */

static long double f1_minus_f3(long double x) { return f1(x) - f3(x); }
static long double f1_minus_f2(long double x) { return f1(x) - f2(x); }

long double compute_area(long double eps)
{
    long double x13 = root(f1, f3, f1d, f3d, -2.0L, -1.0L, eps);
    long double x23 = root(f2, f3, f2d, f3d,  0.0L,  1.0L, eps);
    long double x12 = root(f1, f2, f1d, f2d,  1.0L,  2.0L, eps);

    long double s1 = integral(f1_minus_f3, x13, x23, eps);
    long double s2 = integral(f1_minus_f2, x23, x12, eps);

    return s1 + s2;
}

/* Возвращает три точки пересечения */
void compute_roots(long double eps,
                   long double *x13, long double *x23, long double *x12)
{
    *x13 = root(f1, f3, f1d, f3d, -2.0L, -1.0L, eps);
    *x23 = root(f2, f3, f2d, f3d,  0.0L,  1.0L, eps);
    *x12 = root(f1, f2, f1d, f2d,  1.0L,  2.0L, eps);
}
