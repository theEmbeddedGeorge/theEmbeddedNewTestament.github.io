CC=gcc
CFLGAS=-Wall
OBJ = memalign.o

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

memAlign: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -f memalign memalign.o