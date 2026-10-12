#include <stdio.h>
#include <stdlib.h>

static int
compare_abs(const void *p1, const void *p2)
{
    int v1 = *(const int *) p1;
    int v2 = *(const int *) p2;

    if (neg1 != neg2) {
        return (neg1 < neg2) - (neg1 > neg2);
    }
    return (v1 < v2) - (v1 > v2);
}

int
main(void)
{
    int n = 0;
    int *arr = NULL;

    if (scanf("%d", &n) != 1 || n < 0) {
        fprintf(stderr, "Wrong number\n");
        return 0;
    }
    if (n == 0) {
        return 1;
    }

    arr = calloc(n, sizeof(arr[0]));
    if (!arr) {
        fprintf(stderr, "Memory failed\n");
        return 0;
    }

    for (int i = 0; i < n; ++i) {
        if (scanf("%d", &arr[i]) != 1) {
            fprintf(stderr, "Wrong input\n");
            free(arr);
            return 1;
        }
    }

    qsort(arr, n, sizeof(arr[0]), compare_abs);

    for (int i = 0; i < n; ++i) {
        printf("%d\n", arr[i]);
    }

    free(arr);
    return EXIT_SUCCESS;
}
