CC        = gcc
NASM      = nasm
CFLAGS    = -m32 -Wall -Wextra -Iinclude
LDFLAGS   = -m32 -lm
NASMFLAGS = -f elf32

METHOD ?= NEWTON

SRC = src/main.o src/root.o src/integral.o src/area.o
ASM = asm/funcs.o

all: a.out

a.out: $(SRC) $(ASM)
	$(CC) $(LDFLAGS) -DMETHOD_$(METHOD) $^ -o $@

asm/funcs.o: asm/funcs.asm
	$(NASM) $(NASMFLAGS) $< -o $@

%.o: %.c
	$(CC) $(CFLAGS) -DMETHOD_$(METHOD) -c $< -o $@

clean:
	rm -f src/*.o asm/*.o a.out

.PHONY: all clean
