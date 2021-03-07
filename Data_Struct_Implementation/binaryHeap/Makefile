CC=gcc
CFLGAS=-Wall
OBJ = binaryHeap_2.o

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

binaryHeap_2: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -f  binaryHeap_2 binaryHeap_2.o
