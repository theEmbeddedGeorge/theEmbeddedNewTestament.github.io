CC=gcc
CFLGAS=-Wall -g
OBJ = aob.o

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

aob: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -f aob aob.o