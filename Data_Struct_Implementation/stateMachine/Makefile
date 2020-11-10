CC=gcc
CFLGAS=-Wall
OBJ = stateMachine.o

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

stateMachine: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -f stateMachine stateMachine.o