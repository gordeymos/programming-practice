#include <stdio.h>


enum
{
    UCHAR_MAX = 255,
    BYTE_COUNT = UCHAR_MAX + 1,
    DIGIT_COUNT = 10,
    LETTER_COUNT = 26,
    DIGIT_CODE_START = 1,
    LOWER_CODE_START = DIGIT_CODE_START + DIGIT_COUNT,
    UPPER_CODE_START = LOWER_CODE_START + LETTER_COUNT,
    RESERVED_ZERO_CODE = 0,
    RESERVED_MAX_CODE = UPPER_CODE_START + LETTER_COUNT,
    STUCK_BIT = 2,
    BROKEN_BIT = 3
};

int
main(void)
{
    int c;
    unsigned char tab[BYTE_COUNT];

    for (int i = 0; i < BYTE_COUNT; ++i) {
        int code;

        if (i >= '0' && i <= '9') {
            code = DIGIT_CODE_START + (i - '0');
        } else if (i >= 'a' && i <= 'z') {
            code = LOWER_CODE_START + (i - 'a');
        } else if (i >= 'A' && i <= 'Z') {
            code = UPPER_CODE_START + (i - 'A');
        } else {
            continue;
        }

        int broken = code;

        broken ^= (1 << BROKEN_BIT);
        broken &= ~(1 << STUCK_BIT);

        unsigned char final;

        if (broken == RESERVED_ZERO_CODE) {
            final = '@';
        } else if (broken == RESERVED_MAX_CODE) {
            final = '#';
        } else if (broken >= DIGIT_CODE_START && broken < LOWER_CODE_START) {
            final = '0' + (broken - DIGIT_CODE_START);
        } else if (broken >= LOWER_CODE_START && broken < UPPER_CODE_START) {
            final = 'a' + (broken - LOWER_CODE_START);
        } else {
            final = 'A' + (broken - UPPER_CODE_START);
        }

        tab[i] = final;
    }

    while ((c = getchar_unlocked()) != EOF) {
        if (c >= '0' && c <= '9' || c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z') {
            putchar_unlocked(tab[c]);
        }
    }

    return 0;
}
