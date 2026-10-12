#include <dirent.h>
#include <limits.h>
#include <stdio.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>

int
main(int argc, char *argv[])
{
    if (argc != 2) {
        fprintf(stderr, "usage: %s dir\n", argv[0]);
        return 1;
    }

    DIR *d = opendir(argv[1]);
    if (!d) {
        perror(argv[1]);
        return 1;
    }

    struct dirent *dd = NULL;
    int cout = 0;

    while ((dd = readdir(d)) != NULL) {
        char path[PATH_MAX] = {0};
        int len = snprintf(path, sizeof(path), "%s/%s", argv[1], dd->d_name);

        if (strcmp(dd->d_name, ".") == 0 || strcmp(dd->d_name, "..") == 0) {
            fprintf(stderr, "%s: wrong format\n", dd->d_name);
            continue;
        }

        if (len < 0 || len >= (int) sizeof(path)) {
            fprintf(stderr, "path too long: %s\n", dd->d_name);
            continue;
        }

        struct stat stb = {0};
        if (stat(path, &stb) != 0) {
            perror(path);
            continue;
        }

        if (!S_ISREG(stb.st_mode)) {
            fprintf(stderr, "%s: irregular file\n", dd->d_name);
            continue;
        }

        if (access(path, X_OK) != 0) {
            fprintf(stderr, "%s: non-executable\n", dd->d_name);
            continue;
        }

        size_t lenn = strlen(dd->d_name);

        if (lenn < 4 || strcmp(dd->d_name + lenn - 4, ".exe") != 0) {
            fprintf(stderr, "%s: not .exe\n", dd->d_name);
            continue;
        }
        ++cout;
    }
    closedir(d);
    printf("%d\n", cout);
    return 0;
}
