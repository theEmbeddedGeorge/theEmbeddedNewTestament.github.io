CC=gcc
CFLGAS=-Wall
OBJ = stack.o

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

stack: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -f stack stack.o