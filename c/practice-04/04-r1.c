#include <stdio.h>

unsigned int f(unsigned int n) {
    if (n == 0)
        return 1;
    return f(n - 1) * 3;
}

int main(void) {
    unsigned int n;
    scanf("%u", &n);
    printf("%u\n", f(n));
    return 0;
}