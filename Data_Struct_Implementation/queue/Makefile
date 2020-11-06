CC=gcc
CFLGAS=-Wall
OBJ = queue.o

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

queue: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -f queue queue.o