#include <errno.h>
#include <fcntl.h>
#include <inttypes.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

enum
{
    NODE_FIELDS = 3,
    FIELD_SIZE = 4,
    NODE_SIZE = NODE_FIELDS * FIELD_SIZE,
    LEFT_OFFSET = FIELD_SIZE,
    RIGHT_OFFSET = 2 * FIELD_SIZE,
    STACK_INIT = 16,
    READ_FAILED = -1,
    READ_BAD_DATA = 1,
};

struct Node
{
    int32_t key;
    int32_t left_idx;
    int32_t right_idx;
};

static int32_t
decode_int32(const unsigned char *bytes)
{
    uint32_t value = 0;
    for (int i = 0; i < FIELD_SIZE; ++i) {
        value = value << CHAR_BIT | bytes[i];
    }

    if (value <= INT32_MAX) {
        return (int32_t) value;
    }
    return -(int32_t) (UINT32_MAX - value) - 1;
}

static int
read_node(int fd, int32_t idx, struct Node *node)
{
    if (idx < 0) {
        return READ_BAD_DATA;
    }

    unsigned char buf[NODE_SIZE];
    ssize_t r = pread(fd, buf, sizeof(buf), (off_t) idx * NODE_SIZE);
    if (r == -1) {
        return READ_FAILED;
    }
    if (r != (ssize_t) sizeof(buf)) {
        return READ_BAD_DATA;
    }

    node->key = decode_int32(buf);
    node->left_idx = decode_int32(buf + LEFT_OFFSET);
    node->right_idx = decode_int32(buf + RIGHT_OFFSET);
    return 0;
}

int
main(int argc, char *argv[])
{
    if (argc < 2) {
        fprintf(stderr, "usage: %s FILE\n", argv[0]);
        exit(1);
    }

    int file = open(argv[1], O_RDONLY);
    if (file == -1) {
        fprintf(stderr, "error: %s, %s\n", argv[1], strerror(errno));
        exit(1);
    }

    struct Node *stack = NULL;
    size_t size = 0;
    size_t reserve = 0;

    int32_t idx = 0;
    int active = 1;

    while (active || size > 0) {
        while (active) {
            struct Node node;
            int status = read_node(file, idx, &node);
            if (status == READ_FAILED) {
                perror("pread");
                exit(1);
            }
            if (status == READ_BAD_DATA) {
                fprintf(stderr, "error: %s, invalid data\n", argv[1]);
                exit(1);
            }

            if (size == reserve) {
                reserve = reserve ? reserve * 2 : STACK_INIT;
                struct Node *tmp = realloc(stack, reserve * sizeof(*stack));
                if (!tmp) {
                    perror("realloc");
                    exit(1);
                }
                stack = tmp;
            }
            stack[size++] = node;

            idx = node.right_idx;
            active = idx != 0;
        }

        struct Node top = stack[--size];
        printf("%" PRId32 "\n", top.key);

        idx = top.left_idx;
        active = idx != 0;
    }

    free(stack);
    if (close(file) == -1) {
        perror("close");
        exit(1);
    }
    return 0;
}
