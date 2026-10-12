void
normalize_path(char *buf)
{
    int r = 1;
    int w = 1;

    while (buf[r] != '\0') {
        int e = r;
        while (buf[e] != '/' && buf[e] != '\0') {
            e += 1;
        }
        int len = e - r;
        int last = (buf[e] == '\0');

        if (len == 2 && buf[r] == '.' && buf[r + 1] == '.') {
            if (w > 1) {
                --w;
                while (buf[w - 1] != '/') {
                    --w;
                }
            }
        } else if (!(len == 1 && buf[r] == '.')) {
            for (int i = 0; i < len; ++i) {
                buf[w + i] = buf[r + i];
            }
            w += len;
            buf[w] = '/';
            ++w;
        }

        if (last) {
            break;
        }
        r = e + 1;
    }

    if (w > 1) {
        --w;
    }
    buf[w] = '\0';
}
