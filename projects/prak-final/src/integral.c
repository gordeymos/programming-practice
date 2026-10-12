#include "integral.h"
#include <math.h>

int integral_iters = 0;

/* Формула Симпсона для n разбиений (n чётное) */
static long double simpson(long double (*f)(long double),
                           long double a, long double b, int n)
{
    long double h   = (b - a) / n;
    long double sum = f(a) + f(b);
    int i;

    for (i = 1; i < n; i++) {
        long double x = a + i * h;
        sum += (i % 2 == 0) ? 2.0L * f(x) : 4.0L * f(x);
    }
    return sum * h / 3.0L;
}

/*
 * integral — метод Симпсона с правилом Рунге.
 *
 * Удваиваем n начиная с 4, пока |I(2n) - I(n)| / 15 >= eps.
 * Погрешность метода Симпсона ~ O(h^4), поэтому делитель Рунге = 15.
 */
long double integral(long double (*f)(long double),
                     long double a, long double b, long double eps)
{
    int n    = 4;
    long double I_prev = simpson(f, a, b, n);

    integral_iters = n;

    while (1) {
        n *= 2;
        long double I_curr = simpson(f, a, b, n);

        integral_iters = n;

        if (fabsl(I_curr - I_prev) / 15.0L < eps)
            return I_curr;

        I_prev = I_curr;

        if (n > (1 << 24))   /* защита от зависания */
            break;
    }
    return I_prev;
}
