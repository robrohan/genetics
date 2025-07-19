
clean:
	rm -f gen

build:
	$(CC) -Wall -ggdb \
	-o gen src/main.c
