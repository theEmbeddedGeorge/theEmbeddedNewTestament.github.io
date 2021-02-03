CC=gcc
CFLGAS=-Wall
OBJ = strstr.o

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

stack: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -f strstr strstr.o