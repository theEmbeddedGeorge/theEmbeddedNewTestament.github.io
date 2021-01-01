CC=gcc
CFLGAS=-Wall
OBJ = endianness

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

$(OBJ): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -f $(OBJ) $(OBJ).o
