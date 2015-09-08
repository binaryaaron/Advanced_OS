CC=clang
INCLUDE=$(shell pkg-config --cflags glib-2.0) -I/usr/local/opt/gettext/include

CFLAGS=-g -O2 -Wall -Wextra -Isrc $(INCLUDE) -DNDEBUG $(OPTFLAGS)

LD_FLAGS=-ldl $(OPTLIBS) $(shell pkg-config --libs glib-2.0) -L/usr/local/opt/gettext/lib
PREFIX=./build

SOURCES=$(wildcard src/**/*.c src/*.c)
OBJECTS=$(patsubst %.c,%.o,$(SOURCES))

TEST_SRC=$(wilcard tests/*_tests.c)
TESTS=$(patsubst %.c,%,$(TEST_SRC))

TARGET=build/libunmthread.a
# SO_TARGET=$(patsubst %.a,%.so,$(TARGET))
EXECUTEABLE=threads


#
# The Target Build
all: $(TARGET) $(EXECUTEABLE)

dev: CFLAGS=-g -Wall -Isrc -Wextra $(OPTFLAGS) $(INCLUDE)
dev: all

$(TARGET): CFLAGS += -fPIC
$(TARGET): build $(OBJECTS)
	ar rcs $@ $(OBJECTS)
	ranlib $@

$(SO_TARGET): $(TARGET) $(OBJECTS)
	$(CC) -shared -o $@ $(OBJECTS) -o $(EXECUTEABLE)


$(EXECUTEABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@

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

