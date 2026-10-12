#include <dirent.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

enum
{
    KIB = 1024
};

int
main(int argc, char *argv[])
{
    long long ans = 0;

    for (int i = 1; i < argc; ++i) {
        struct stat stb = {0};
        if (lstat(argv[i], &stb) < 0) {
            continue;
        }

        if (S_ISREG(stb.st_mode) && stb.st_nlink == 1 && stb.st_size % KIB == 0) {
            ans += (long long) stb.st_size;
        }
    }

    printf("%lld\n", ans);

    return 0;
}
