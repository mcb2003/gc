CFLAGS = -Wall -Wextra -Wsign-conversion

SOURCES = $(shell ls *.c)
OBJECTS = ${SOURCES:.c=.o}
	HEADERS = $(shell ls *.h)
BINARY = gc

.PHONY: all release clean

all: CFLAGS += -DDEBUG -O0 -g
all: ${BINARY}

release: CFLAGS += -O3
release: ${BINARY}
	strip ${BINARY}

${BINARY}: ${OBJECTS}
	${LINK.c} ${OUTPUT_OPTION} $^

clean:
	${RM} ${BINARY} ${OBJECTS}
