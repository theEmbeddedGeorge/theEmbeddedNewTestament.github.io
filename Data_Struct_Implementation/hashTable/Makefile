CC=gcc
CFLGAS=-Wall
OBJ = hashTable.o

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

hashTable: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -f hashTable hashTable.o