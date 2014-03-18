IDIR = .
CC = gcc
CFLAGS = -I$(IDIR) -g -Wall
_DEPS = hw5.h
DEPS = $(pathsubst %, $(IDIR)/%, $(_DEPS))

OBJ = hw5.o encrypt.o decrypt.o tablecheck.o util.o

hw5: $(OBJ)
	$(CC) $^ -o $@

./%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

.PHONY: clean

clean:
	rm -f ./*.o hw5
