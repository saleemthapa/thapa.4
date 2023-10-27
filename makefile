# Compiler and compiler flags
CC = gcc
CFLAGS = -Wall

# Executable names
OSS_EXEC = oss
WORKER_EXEC = worker

all: $(OSS_EXEC) $(WORKER_EXEC)

$(OSS_EXEC): oss.c
	$(CC) $(CFLAGS) -o $@ oss.c

$(WORKER_EXEC): worker.c
	$(CC) $(CFLAGS) -o $@ worker.c

clean:
	rm -f $(OSS_EXEC) $(WORKER_EXEC)
