CC=gcc
INCLUDE=$(shell pkg-config --cflags glib-2.0)

CFLAGS=-g -std=gnu99 -O2 -Wall -Isrc -DNDEBUG $(OPTFLAGS) $(INCLUDE)

LDFLAGS=-ldl $(OPTLIBS) $(shell pkg-config --libs glib-2.0)
PREFIX=./build

SOURCES=$(wildcard src/**/*.c src/*.c)
OBJECTS=$(patsubst %.c,%.o,$(SOURCES))

TEST_SRC=$(wilcard tests/*_tests.c)
TESTS=$(patsubst %.c,%,$(TEST_SRC))

TARGET=build/libunmthread.a
#SO_TARGET=$(patsubst %.a,%.so,$(TARGET))
EXECUTEABLE=threads


all: $(TARGET) $(EXECUTEABLE)

dev: CFLAGS=-g -std=gnu99 -Wall -Isrc -Wextra -DEBUG $(OPTFLAGS) $(INCLUDE)
dev: all

$(TARGET): CFLAGS += -fPIC
$(TARGET): build $(OBJECTS)
	ar rcs $@ $(OBJECTS)
	ranlib $@

$(SO_TARGET): $(TARGET) $(OBJECTS)
	$(CC) $(EXECUTEABLE) -shared -o $@ $(OBJECTS) -o 


$(EXECUTEABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

build:
	@mkdir -p build
	@mkdir -p bin

# The unit tests
.PHONY: tests
tests: CFLAGS += $(TARGET)
tests: $(TESTS)
	@echo not testing

valgrind:
	VALGRIND="valgrind --log-file=/tmp/valgrind-%p.log" $(MAKE)

clean:
	rm -rf build $(OBJECTS) $(TESTS)
	rm -f tests/tests.lob
	rm $(EXECUTEABLE)
	find . -name "*.gc*" -exec rm {} \;
	rm -rf `find . -name "*.dSYM" -print`

install: all
	install -d $(DESTDIR)/$(PREFIX)/lib/
	install $(TARGET) $(DESTDIR)/$(PREFIX)/lib

BADFUNCS='[^_.>a-zA-Z0-9](str(n?cpy|n?cat|xfrm|n?dup|str|pbrk|tok|_)|stpn?cpy|a?sn?printf|byte_)'

check:
	@echo Files with potentially dangerous functions.
	@egrep $(BADFUNCS) $(SOURCES) || true
	@cd tests && make && make check

