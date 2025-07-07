# Makefile to build avr_sh target

CXX=clang
CFLAGS = -g -Wall -Wextra -Werror -pedantic-errors

DEPS=

TARGET=avr_sh

OBJ=${TARGET}.o

all: ${TARGET}

${TARGET}: ${OBJ}
	${CXX} -o $@ $^ ${CFLAGS}

%.o: %.c ${DEPS}
	${CXX} -c -o $@ $< ${CFLAGS}

clean:
	rm -rf *.o ${TARGET}


# $@ - Target
# $^ - All prereqs
# $< - First prereq