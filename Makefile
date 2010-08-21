CFLAGS=-g -Wall -Wshadow -Isrc

SOURCES=$(wildcard src/*.c src/**/*.c)
#OBJECTS=$(patsubst %.c,%.o,${SOURCES})
OBJECTS=$(SOURCES:.c=.o)
TEST_SRC=$(wildcard tests/*.c)
TESTS=$(patsubst %.c,%,${TEST_SRC})

all: build bin/loldrop tests

release: CFLAGS=-g -Os -Wall -Isrc -DNDEBUG
release: all

bin/loldrop: $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $@

build:
	@mkdir -p bin

clean:
	rm -rf bin ${OBJECTS} ${TESTS}
	rm -rf tests/m2.zcov tests/coverage
	find . -name "*.gc*" -exec rm {} \;

tests: ${TESTS}
	sh ./tests/runtests.sh

$(TESTS): %: %.c
	$(CC) $(CFLAGS) $(LIBS) $(filter-out src/main.o, $(OBJECTS)) -o $@ $<

src/state_machine.o: ragel
src/state_machine.c: src/state_machine.rl

check:
	@echo Files with potentially dangerous functions.
	@egrep '[^_.>a-zA-Z0-9](str(n?cpy|n?cat|xfrm|n?dup|str|pbrk|tok|_)|stpn?cpy|a?sn?printf|byte_)' $(filter-out src/bstr/bstraux.c src/bstr/bsafe.c,${SOURCES})

ragel:
	ragel -G2 src/state_machine.rl

valgrind:
	valgrind --leak-check=full --show-reachable=yes --log-file=valgrind.log --suppressions=tests/valgrind.sup ./bin/loldrop

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

coverage: CFLAGS += -fprofile-arcs -ftest-coverage
coverage: clean all
	zcov-scan tests/m2.zcov
	zcov-genhtml tests/m2.zcov tests/coverage
	zcov-summarize tests/m2.zcov
