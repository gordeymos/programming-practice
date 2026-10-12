#include <stdio.h>

enum
{
    MAX_LENGHT = 128,
    FINAL = MAX_LENGHT + 1
};

int
main(void)
{
    char s1[FINAL], s2[FINAL], s3[FINAL];

    if (scanf("%128s%128s%128s", s1, s2, s3) != 3) {
        fprintf(stderr, "Wrong input");
        return 1;
    }

    printf("[Host:%s,Login:%s,Password:%s]\n", s1, s2, s3);

    return 0;
}
