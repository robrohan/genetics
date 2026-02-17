.PHONY: test_nn test_gen

CFLAGS := -std=c99 -Wall -Wextra -Wshadow -Wpointer-arith -Wcast-align \
 -Wstrict-prototypes -Wmissing-prototypes -Wmissing-declarations \
 -Wdouble-promotion -Wswitch-enum -Wimplicit-fallthrough \
 -Wconversion -Wsign-conversion -Wundef -Werror
CC := clang


clean:
	rm -f gen
	rm -f test_gen
	rm -f test_nn
	rm -rf *.dSYM

test_nn:
	$(CC) $(CFLAGS) -ggdb -o test_nn \
		src/nn.c src/test_nn.c
	./test_nn

test_gen:
	$(CC) $(CFLAGS) -ggdb -o test_gen \
		src/gen.c src/test_gen.c
	./test_gen

test_train:
	$(CC) $(CFLAGS) -ggdb -o gen_train \
		src/gen.c src/nn.c src/gen_train.c
	./gen_train


test: test_nn test_gen
	@echo "Done"
