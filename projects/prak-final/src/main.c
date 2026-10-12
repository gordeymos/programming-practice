#include <stdio.h>
#include <string.h>
#include <math.h>

#include "funcs.h"
#include "root.h"
#include "integral.h"
#include "area.h"

/* ------------------------------------------------------------------ */
/* Вспомогательные функции для тестов (стандартные math.h)             */
/* ------------------------------------------------------------------ */

static long double tst_sin(long double x)  { return sinl(x);  }
static long double tst_cos(long double x)  { return cosl(x);  }
static long double tst_zero(long double x) { (void)x; return 0.0L; }

/* f(x) = x^2, g(x) = 2  =>  корень = sqrt(2) */
static long double tst_xsq(long double x)  { return x * x; }
static long double tst_2xd(long double x)  { return 2.0L * x; }
static long double tst_two(long double x)  { (void)x; return 2.0L; }

/* f(x) = e^x, g(x) = 3  =>  корень = ln(3) */
static long double tst_exp(long double x)  { return expl(x);  }
static long double tst_expd(long double x) { return expl(x);  }
static long double tst_three(long double x){ (void)x; return 3.0L; }

/* ------------------------------------------------------------------ */
/* Интегрируемые функции для тестов                                    */
/* ------------------------------------------------------------------ */

/* ∫[0,1] x^2 dx = 1/3 */
static long double itst_x2(long double x)  { return x * x; }

/* ∫[0,π] sin(x) dx = 2 */
static long double itst_sin(long double x) { return sinl(x); }

/* ∫[0,1] x^3 dx = 1/4 */
static long double itst_x3(long double x)  { return x * x * x; }

/* ------------------------------------------------------------------ */

static void print_help(void)
{
    puts("Лабораторная работа №6, вариант 10. Москвин Г.Е.");
    puts("Функции: f1=1+4/(x^2+1),  f2=x^3,  f3=2^(-x)");
    puts("");
    puts("Использование: ./a.out [опция]");
    puts("  (без опций)      — площадь фигуры при eps=1e-7");
    puts("  --help           — справка");
    puts("  --roots          — точки пересечения кривых");
    puts("  --iter           — число итераций (корни + интегралы)");
    puts("  --test-root      — тесты функции root()");
    puts("  --test-integral  — тесты функции integral()");
}

static void print_roots(long double eps)
{
    long double x13, x23, x12;
    compute_roots(eps, &x13, &x23, &x12);

    printf("Точки пересечения (eps = %.0Le):\n", eps);
    printf("  f1 ∩ f3:  x = %20.15Lf  y = %20.15Lf\n", x13, f1(x13));
    printf("  f2 ∩ f3:  x = %20.15Lf  y = %20.15Lf\n", x23, f2(x23));
    printf("  f1 ∩ f2:  x = %20.15Lf  y = %20.15Lf\n", x12, f1(x12));
}

static void print_iters(long double eps)
{
    long double x13, x23, x12;

    x13 = root(f1, f3, f1d, f3d, -2.0L, -1.0L, eps);
    printf("root(f1,f3) итераций: %d\n", root_iters);

    x23 = root(f2, f3, f2d, f3d, 0.0L, 1.0L, eps);
    printf("root(f2,f3) итераций: %d\n", root_iters);

    x12 = root(f1, f2, f1d, f2d, 1.0L, 2.0L, eps);
    printf("root(f1,f2) итераций: %d\n", root_iters);

    /* Один пример интеграла */
    integral(f1, x13, x23, eps);
    printf("integral(f1,[x13,x23]) n разбиений: %d\n", integral_iters);
    (void)x12;
}

/* ------------------------------------------------------------------ */
/* Тесты root()                                                        */
/* ------------------------------------------------------------------ */
static void test_root(void)
{
    long double res, err;
    long double eps = 1e-9L;

#ifdef METHOD_NEWTON
    puts("=== Тест root() — метод Ньютона ===");
#endif
#ifdef METHOD_COMBINED
    puts("=== Тест root() — комбинированный метод ===");
#endif

    /* Тест 1: sin(x) = 0 на [3, 4]  =>  x = π */
    res = root(tst_sin, tst_zero, tst_cos, tst_zero, 3.0L, 4.0L, eps);
    err = fabsl(res - (long double)M_PI);
    printf("  sin(x)=0 на [3,4]: x=%.15Lf  |err|=%.3Le  iters=%d\n",
           res, err, root_iters);

    /* Тест 2: x^2 = 2 на [1, 2]  =>  x = √2 */
    res = root(tst_xsq, tst_two, tst_2xd, tst_zero, 1.0L, 2.0L, eps);
    err = fabsl(res - sqrtl(2.0L));
    printf("  x^2=2 на [1,2]:   x=%.15Lf  |err|=%.3Le  iters=%d\n",
           res, err, root_iters);

    /* Тест 3: e^x = 3 на [0, 2]  =>  x = ln(3) */
    res = root(tst_exp, tst_three, tst_expd, tst_zero, 0.0L, 2.0L, eps);
    err = fabsl(res - logl(3.0L));
    printf("  e^x=3 на [0,2]:   x=%.15Lf  |err|=%.3Le  iters=%d\n",
           res, err, root_iters);
}

/* ------------------------------------------------------------------ */
/* Тесты integral()                                                    */
/* ------------------------------------------------------------------ */
static void test_integral(void)
{
    long double res, err;
    long double eps = 1e-9L;

    puts("=== Тест integral() — метод Симпсона (правило Рунге) ===");

    /* Тест 1: ∫[0,1] x^2 dx = 1/3 */
    res = integral(itst_x2, 0.0L, 1.0L, eps);
    err = fabsl(res - 1.0L/3.0L);
    printf("  ∫x^2 [0,1]:   res=%.15Lf  точно=%.15Lf  |err|=%.3Le  n=%d\n",
           res, 1.0L/3.0L, err, integral_iters);

    /* Тест 2: ∫[0,π] sin(x) dx = 2 */
    res = integral(itst_sin, 0.0L, (long double)M_PI, eps);
    err = fabsl(res - 2.0L);
    printf("  ∫sin[0,π]: res=%.15Lf  точно=%.15Lf  |err|=%.3Le  n=%d\n",
           res, 2.0L, err, integral_iters);

    /* Тест 3: ∫[0,1] x^3 dx = 1/4 */
    res = integral(itst_x3, 0.0L, 1.0L, eps);
    err = fabsl(res - 0.25L);
    printf("  ∫x^3 [0,1]:   res=%.15Lf  точно=%.15Lf  |err|=%.3Le  n=%d\n",
           res, 0.25L, err, integral_iters);
}

/* ------------------------------------------------------------------ */
/* main                                                                */
/* ------------------------------------------------------------------ */
int main(int argc, char *argv[])
{
    if (argc == 1) {
        /* Основной результат: площадь при трёх значениях ε */
        static const long double eps_vals[] = {1e-5L, 1e-7L, 1e-9L};
        int i;
        printf("Площадь фигуры (вариант 10):\n");
        for (i = 0; i < 3; i++) {
            long double eps = eps_vals[i];
            long double s   = compute_area(eps);
            printf("  eps = %.0Le  =>  S = %.15Lf\n", eps, s);
        }
        return 0;
    }

    if (strcmp(argv[1], "--help") == 0) {
        print_help();
    } else if (strcmp(argv[1], "--roots") == 0) {
        print_roots(1e-9L);
    } else if (strcmp(argv[1], "--iter") == 0) {
        print_iters(1e-9L);
    } else if (strcmp(argv[1], "--test-root") == 0) {
        test_root();
    } else if (strcmp(argv[1], "--test-integral") == 0) {
        test_integral();
    } else {
        fprintf(stderr, "Неизвестная опция: %s\n", argv[1]);
        fprintf(stderr, "Используйте --help для справки.\n");
        return 1;
    }
    return 0;
}
