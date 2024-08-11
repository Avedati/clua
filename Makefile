CC=gcc
CFLAGS=-std=c11
TARGETS=src/*.c
OUT=bin/clua

all:
	$(CC) $(CFLAGS) $(TARGETS) -o $(OUT)

clean:
	rm -f $(OUT)

test:
	$(MAKE) all
	./$(OUT)
	$(MAKE) clean
