
CFLAGS := -std=c99 -Wall -Wextra -Wshadow -Wpointer-arith -Wcast-align \
 -Wstrict-prototypes -Wmissing-prototypes -Wmissing-declarations \
 -Wdouble-promotion -Wswitch-enum -Wimplicit-fallthrough \
 -Wconversion -Wsign-conversion -Wundef -Werror
CC := clang

build:
	$(CC) $(CFLAGS) -ggdb \
	-o gen src/gen.c

clean:
	rm -f gen

