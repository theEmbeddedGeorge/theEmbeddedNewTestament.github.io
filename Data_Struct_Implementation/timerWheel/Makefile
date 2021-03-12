CC=gcc
CFLGAS=-Wall
OBJ = timer.o

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

timer: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -f timer timer.o
