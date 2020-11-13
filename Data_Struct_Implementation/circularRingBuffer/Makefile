CC=gcc
CFLGAS=-Wall
DEPS = ring_buffer.h
OBJ = ring_buffer.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

ring_buffer: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -f ring_buffer ring_buffer.o