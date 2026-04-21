# Makefile for sparse-integer-linalg
#
# Usage:
#   make          — build everything
#   make test     — build and run all tests
#   make clean    — remove build artifacts

CC      = gcc
# -Wall -Wextra: enable all warnings (good practice)
# -std=c99:      use C99 standard
# -g:            include debug symbols
CFLAGS  = -Wall -Wextra -std=c99 -g -I include

# Source files shared by both test binaries
SRC     = src/csr.c src/arithmetic.c

# Test binaries
TEST_CSR   = test_csr
TEST_ARITH = test_arithmetic

.PHONY: all test clean

all: $(TEST_CSR) $(TEST_ARITH)

$(TEST_CSR): $(SRC) tests/test_csr.c
	$(CC) $(CFLAGS) -o $@ $(SRC) tests/test_csr.c

$(TEST_ARITH): $(SRC) tests/test_arithmetic.c
	$(CC) $(CFLAGS) -o $@ $(SRC) tests/test_arithmetic.c

test: all
	@echo "--- Running CSR tests ---"
	@./$(TEST_CSR)
	@echo "--- Running arithmetic tests ---"
	@./$(TEST_ARITH)
	@echo "All tests passed."

clean:
	rm -f $(TEST_CSR) $(TEST_ARITH)
