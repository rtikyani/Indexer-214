CC=gcc
CFLAGS= -Wall -O -g

malloc: src/main.c
	$(CC) $(CFLAGS) src/main.c src/util.c src/util.h src/entry.c src/entry.h src/list.c src/list.h src/tokenizer.c src/tokenizer.h src/indexer.c src/indexer.h -o indexer

clean:
	rm indexer
