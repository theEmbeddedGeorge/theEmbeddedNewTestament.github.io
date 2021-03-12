CC=gcc
CFLGAS=-Wall
OBJ = timer.o

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

stack: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -f timer timer.o