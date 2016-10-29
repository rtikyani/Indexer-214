#ifndef _INDEXER_H_
#define _INDEXER_H_

#include "list.h"

typedef struct indexer {
    list_t *items;
} indexer_t;

indexer_t *create_indexer();

void free_indexer(indexer_t *);

int run_indexer(indexer_t *, char *);

typedef struct entry {
    char *string;
    list_t *list;
} entry_t;

entry_t *create_entry(char *);

void free_entry(entry_t *);

#endif