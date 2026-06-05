all: test_pam

test_pam:
	@echo "+====================+"
	@echo "|  Building test_01  |"
	@echo "+====================+"
	mkdir -p build
	cp -r sprite build/
	clang -g -Wall -Wextra -o build/test_pam test/test_pam.c easydisplib.c -I.
	@echo "+====================+"
	@echo "|   Build Finished   |"
	@echo "+====================+"
	@echo ""

run_pam:
	build/test_pam

clean:
	rm -rf build/*
