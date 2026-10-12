#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

enum
{
    BUF_COUNT = 512
};

static ssize_t
read_full(int fd, void *buf, size_t size)
{
    size_t done = 0;
    while (done < size) {
        ssize_t r = read(fd, (char *) buf + done, size - done);
        if (r == -1) {
            return -1;
        }
        if (r == 0) {
            break;
        }
        done += (size_t) r;
    }
    return (ssize_t) done;
}

static int
write_full(int fd, const void *buf, size_t size)
{
    size_t done = 0;
    while (done < size) {
        ssize_t w = write(fd, (const char *) buf + done, size - done);
        if (w == -1) {
            return -1;
        }
        done += (size_t) w;
    }
    return 0;
}

int
main(int argc, char *argv[])
{
    if (argc < 2) {
        fprintf(stderr, "usage: %s FILE\n", argv[0]);
        exit(1);
    }

    int file = open(argv[1], O_RDWR);
    if (file == -1) {
        fprintf(stderr, "error: %s, %s\n", argv[1], strerror(errno));
        exit(1);
    }

    long long min;
    ssize_t got = read_full(file, &min, sizeof(min));
    if (got == -1) {
        perror("read");
        exit(1);
    }
    if (got != 0 && got != (ssize_t) sizeof(min)) {
        fprintf(stderr, "error: %s, size is not a multiple of %zu\n", argv[1], sizeof(min));
        exit(1);
    }

    if (got != 0) {
        off_t min_pos = 0;
        off_t pos = (off_t) sizeof(min);
        long long buf[BUF_COUNT];

        while ((got = read_full(file, buf, sizeof(buf))) > 0) {
            if ((size_t) got % sizeof(buf[0]) != 0) {
                fprintf(stderr, "error: %s, size is not a multiple of %zu\n", argv[1], sizeof(buf[0]));
                exit(1);
            }

            size_t count = (size_t) got / sizeof(buf[0]);
            for (size_t i = 0; i < count; ++i) {
                if (buf[i] < min) {
                    min = buf[i];
                    min_pos = pos;
                }
                pos += (off_t) sizeof(buf[0]);
            }
        }
        if (got == -1) {
            perror("read");
            exit(1);
        }

        unsigned long long result = -(unsigned long long) min;

        if (lseek(file, min_pos, SEEK_SET) == -1) {
            perror("lseek");
            exit(1);
        }

        if (write_full(file, &result, sizeof(result)) == -1) {
            perror("write");
            exit(1);
        }
    }

    if (close(file) == -1) {
        perror("close");
        exit(1);
    }
    return 0;
}
