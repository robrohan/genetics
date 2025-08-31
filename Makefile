
CFLAGS := -Wall -Wextra -Wshadow -Wpointer-arith -Wcast-align \
 -Wstrict-prototypes -Wmissing-prototypes -Wmissing-declarations \
 -Wdouble-promotion -Wswitch-enum -Wimplicit-fallthrough \
 -Wconversion -Wsign-conversion -Wundef -Werror

clean:
	rm -f gen

build:
	$(CC) $(CFLAGS) -ggdb \
	-o gen src/gen.c

gd:
	$(CC) $(CFLAGS) -ggdb \
		-o gd src/gd.c

sign:
	codesign -s - -f --entitlements Entitlements.plist gen

