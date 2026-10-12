#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

enum
{
    PENSIEVE_BYTE_BITS = 12,
    PENSIEVE_BYTE_MASK = 0xFFF,
    UPPER_PART_SHIFT = 8,
    UPPER = 0xF,
    LOWER = 0xFF,
    OUTPUT = 4,
};

int
main(int argc, char *argv[])
{
    int ans = -1;

    if (argc < 2) {
        return 1;
    }

    ans = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, 0600);
    if (ans == -1) {
        return 1;
    }

    unsigned int n;
    while (scanf("%u", &n) == 1) {
        unsigned int high = (n >> PENSIEVE_BYTE_BITS) & PENSIEVE_BYTE_MASK;
        unsigned int low = n & PENSIEVE_BYTE_MASK;

        unsigned char output[OUTPUT];
        output[0] = (high >> UPPER_PART_SHIFT) & UPPER;
        output[1] = high & LOWER;
        output[2] = (low >> UPPER_PART_SHIFT) & UPPER;
        output[3] = low & LOWER;

        size_t done = 0;
        while (done < sizeof(output)) {
            ssize_t written = write(ans, output + done, sizeof(output) - done);
            if (written <= 0) {
                close(ans);
                return 1;
            }
            done += (size_t) written;
        }
    }

    if (close(ans) == -1) {
        return 1;
    }
    return 0;
}
