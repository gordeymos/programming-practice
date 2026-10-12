#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

enum
{
    DECIMAL_BASE = 10,
    MIN_COUNT = 2,
};

static ssize_t
read_full(int fd, void *buf, size_t size)
{
    size_t ans = 0;
    while (ans < size) {
        ssize_t r = read(fd, (char *) buf + ans, size - ans);
        if (r == -1) {
            return -1;
        }
        if (r == 0) {
            break;
        }
        ans += (size_t) r;
    }
    return (ssize_t) ans;
}

int
main(int argc, char *argv[])
{
    if (argc < 3) {
        fprintf(stderr, "usage: %s FILE N\n", argv[0]);
        exit(0);
    }

    char *eptr = NULL;
    errno = 0;
    long count = strtol(argv[2], &eptr, DECIMAL_BASE);
    if (errno || *eptr || eptr == argv[2]) {
        fprintf(stderr, "invalid number: %s\n", argv[2]);
        exit(0);
    }

    if (count < MIN_COUNT) {
        return 0;
    }

    int file = open(argv[1], O_RDWR);
    if (file == -1) {
        fprintf(stderr, "error: %s, %s\n", argv[1], strerror(errno));
        exit(0);
    }

    double out;
    ssize_t r = read_full(file, &out, sizeof(out));
    if (r == -1) {
        perror("read");
        exit(0);
    }

    if (r == (ssize_t) sizeof(out)) {
        for (long i = 1; i < count; ++i) {
            double cur;
            r = read_full(file, &cur, sizeof(cur));
            if (r == -1) {
                perror("read");
                exit(0);
            }
            if (r != (ssize_t) sizeof(cur)) {
                break;
            }

            cur -= out;

            if (lseek(file, -(off_t) sizeof(cur), SEEK_CUR) == -1) {
                perror("lseek");
                exit(0);
            }

            size_t ans = 0;
            while (ans < sizeof(cur)) {
                ssize_t written = write(file, (char *) &cur + ans, sizeof(cur) - ans);
                if (written == -1) {
                    perror("write");
                    exit(0);
                }
                ans += (size_t) written;
            }

            out = cur;
        }
    }

    if (close(file) == -1) {
        perror("close");
        exit(0);
    }
    return 0;
}
