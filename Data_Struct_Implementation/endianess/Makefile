CC=gcc
CFLGAS=-Wall
OBJ = endianness

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

memAlign: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -f $(OBJ) $(OBJ).o