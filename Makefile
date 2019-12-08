CC ?= gcc

all: pdfjpg.c
	$(CC) pdfjpg.c -o pdfjpg

clean:
	rm -f pdfjpg

