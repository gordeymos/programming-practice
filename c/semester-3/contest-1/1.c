#include <stdio.h>

int
main()
{
    double x, y;
    if (scanf("%lf %lf", &x, &y) == 2){
        if (y >= 1 && y <= 7 && x >= 2 && x <= 3) {
        printf("%d\n", 1);
        } else if (y >= 3 && y <= 7 && x >= 3 && x <= 5) {
            printf("%d\n", 1);
        } else if (y >= 1 && y <= 3 && x >= 3 && x <= 5 && (x - 2 <= y)) {
            printf("%d\n", 1);
        } else {
            printf("%d\n", 0);
        }
    } esle {
        printf ("Wrong input");
    }
    
    return 0;
}
