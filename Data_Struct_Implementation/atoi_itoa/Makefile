CC=gcc
CFLGAS=-Wall
DEPS = 
OBJ = atoi.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

atoi: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -f atoi atoi.o