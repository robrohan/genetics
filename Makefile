
clean:
	rm -f gen

build:
	$(CC) -Wall -ggdb \
	-o gen src/main.c

sign:
	codesign -s - -f --entitlements Entitlements.plist gen

