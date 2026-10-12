#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum
{
    BITS = 9
};

static const char obr[] = "rwxrwxrwx";

int
main(int argc, char *argv[])
{
    for (int i = 1; i < argc; ++i) {
        unsigned mode = (unsigned) strtoul(argv[i], NULL, 8);

        char buf[sizeof(obr)] = {0};
        strcpy(buf, obr);

        for (int j = 0; j < BITS; ++j) {
            if ((mode & (1u << (BITS - 1 - j))) == 0) {
                buf[j] = '-';
            }
        }

        printf("%s\n", buf);
    }

    return 0;
}
