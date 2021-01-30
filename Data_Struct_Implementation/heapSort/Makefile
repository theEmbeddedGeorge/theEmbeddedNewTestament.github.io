CC=gcc
CFLGAS=-Wall
OBJ = heapSort

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

$(OBJ): $(OBJ).o
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -f $(OBJ) $(OBJ).o