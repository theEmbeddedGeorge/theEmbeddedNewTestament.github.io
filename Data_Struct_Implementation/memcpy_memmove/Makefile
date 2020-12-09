CC=gcc
CFLGAS=-Wall
DEPS = memcpy.h
OBJ = memcpy.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

memcpy: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -f memcpy memcpy.o