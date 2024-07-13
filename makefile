CC = gcc
CFLAGS = -Wall

# List of source files
SRCS = memorysystem.c

TARGETS = $(SRCS:.c=)

.PHONY: all clean rebuild run

all: $(TARGETS)

memorysystem: memorysystem.c
	$(CC) $(CFLAGS) -o memorysystem memorysystem.c

clean:
	rm -f $(TARGETS)

rebuild: clean all

run: all
	./memorysystem