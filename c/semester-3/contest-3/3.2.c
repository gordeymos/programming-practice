#include <stdio.h>
//перепиать
int
main(void)
{
    int perem;
    int bol = 0;
    int men = 0;

    while (scanf("%d", &perem) != EOF) {
        if (perem > 0) {
            bol += perem;
        } else {
            men += perem;
        }
    }

    printf ("%d\n%d\n", bol, men);
    return 0;
}

