CC=gcc
CFLGAS=-Wall
OBJ = quicksort.o

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

quicksort: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -f  quicksort quicksort.o