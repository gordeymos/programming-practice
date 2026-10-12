#include <stdio.h>
#include <stdlib.h>

static constexpr int MAX_N = 1000000

int cmp(const void *p1, const void *p2) //стандартная функция сравнения
{
    int v1 = *(int*)p1;
    int v2 = *(int*)p2;
    if (v1 > 0) {
        v1 = -v1;
    }
    if (v2 > 0) {
        v2 = -v2;
    }

    return (v1 > v2) - (v1 - v2);
}

int
main()
{
    int n;
    scanf("%d", &n); //MUST CHECK
    if (n <=0 || n>=MAX_N) {
        exit(1);
    }

    //int arr[n]
    int *ptr = calloc(n, sizeof(*ptr));
    if (!ptr) exit(1);

}
