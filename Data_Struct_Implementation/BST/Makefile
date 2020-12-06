CC=gcc
CFLGAS=-Wall -g
OBJ = bst.o

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

bst: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -f bst bst.o