all: test_01 pong

test_01:
	@echo "+====================+"
	@echo "|  Building test_01  |"
	@echo "+====================+"
	mkdir -p build
	cp -r sprite build/
	gcc -g -Wall -Wextra -o build/test_01 test/test_01.c easydisplib.c -I.
	@echo "+====================+"
	@echo "|   Build Finished   |"
	@echo "+====================+"
	@echo ""

pong:
	@echo "+====================+"
	@echo "|   Building Pong    |"
	@echo "+====================+"
	mkdir -p build
	gcc -g -Wall -Wextra -o build/pong test/pong.c easydisplib.c -I.
	@echo "+====================+"
	@echo "|   Build Finished   |"
	@echo "+====================+"
	@echo ""

run:
	build/test_01

run-pong:
	build/pong

clean:
	rm -rf build/*
