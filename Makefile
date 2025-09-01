
CFLAGS := -std=c99 -Wall -Wextra -Wshadow -Wpointer-arith -Wcast-align \
 -Wstrict-prototypes -Wmissing-prototypes -Wmissing-declarations \
 -Wdouble-promotion -Wswitch-enum -Wimplicit-fallthrough \
 -Wconversion -Wsign-conversion -Wundef
# -Werror
CC := clang

clean:
	rm -f gen
	rm -f gd

build:
	$(CC) $(CFLAGS) -ggdb \
	-o gen src/gen.c

sign:
	codesign -s - -f --entitlements Entitlements.plist gen

