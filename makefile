# Compiler and compiler flags
CC = gcc
CFLAGS = -Wall

# Executable names
OSS_EXEC = oss
WORKER_EXEC = worker

# Additional source files
MYTIME_SRC = mytime.c

all: $(OSS_EXEC) $(WORKER_EXEC)

$(OSS_EXEC): oss.c
	$(CC) $(CFLAGS) -o $@ oss.c

$(WORKER_EXEC): worker.c
	$(CC) $(CFLAGS) -o $@ worker.c	$(MYTIME_SRC)	-lrt

clean:
	rm -f $(OSS_EXEC) $(WORKER_EXEC)
