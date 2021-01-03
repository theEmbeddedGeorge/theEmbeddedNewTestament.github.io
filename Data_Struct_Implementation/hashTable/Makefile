CC=gcc
CFLGAS=-Wall
OBJ = hashTable
OBJ2 = hashTable_chain

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

$(OBJ): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

$(OBJ2): $(OBJ2)
	$(CC) -o $(OBJ2) $(OBJ2).c $(CFLAGS)

clean:
	rm -f $(OBJ) $(OBJ).o
	rm -f $(OBJ2) $(OBJ2).o
