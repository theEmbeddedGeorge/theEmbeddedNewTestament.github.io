CC = gcc
CFLGAS=-I.
CCFLAGS=-lrt
DEPS = fan_hw.h fan_control.h
OBJ_CLIENT = fan_control_client
OBJ_SERVER = fan_control_server
EXECUTABLE = $(OBJ_CLIENT) $(OBJ_SERVER)

all: $(EXECUTABLE)

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS) $(CCFLAGS)

$(OBJ_CLIENT): $(OBJ_CLIENT).o
	$(CC) -o $@ $^ $(CFLAGS) $(CCFLAGS)

$(OBJ_SERVER): $(OBJ_SERVER).o
	$(CC) -o $@ $^ $(CFLAGS) $(CCFLAGS)

clean:
	@echo "Cleaning up..."
	rm -rvf *.o $(OBJ_CLIENT) $(OBJ_SERVER)
