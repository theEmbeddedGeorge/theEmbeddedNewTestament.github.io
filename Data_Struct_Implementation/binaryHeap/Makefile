CC=gcc
CFLGAS=-Wall
OBJ = binaryHeap.o

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

binaryHeap: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -f  binaryHeap binaryHeap.o