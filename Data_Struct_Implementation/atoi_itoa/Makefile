CC=gcc
CFLGAS=-Wall
DEPS = 
OBJ = atoi_iota.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

atoi_iota: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -f atoi_iota atoi_iota.o