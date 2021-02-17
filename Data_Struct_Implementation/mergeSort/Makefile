CC=gcc
CFLGAS=-Wall
DEPS = 
OBJ = mergesort

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(OBJ): $(OBJ).o
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -f $(OBJ) $(OBJ).o