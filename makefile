all: test_01

test_01:
	gcc -g -Wall -Wextra -o build/test_01 test/test_01.c

run:
	build/test_01

clean:
	rm build/*
