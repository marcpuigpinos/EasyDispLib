all: test_01

test_01:
	mkdir -p build
	cp -r sprite build/
	gcc -g -Wall -Wextra -o build/test_01 test/test_01.c easydisplib.c -I.

run:
	build/test_01

clean:
	rm -rf build/*
