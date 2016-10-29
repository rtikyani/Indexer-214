#include <stdlib.h>
#include <string.h>
#include "entry.h"
#include "indexer.h"

entry_record_t *create_entry_record(char *path) {
    entry_record_t *record = malloc(sizeof(entry_record_t));

    record->path = malloc(strlen(path) + sizeof(char));

    strcpy(record->path, path);
    record->amount = 0;

    return record;
}

void free_entry_record(entry_record_t *record) {
    free(record->path);
    free(record);
}

static int record_compare_function(void *first, void *second) {
    entry_record_t *first_record = first;
    entry_record_t *second_record = second;

    return first_record->amount < second_record->amount ? -1 : 1;
}

static void record_free_function(void *object) {
    free_entry_record(object);
}

entry_t *create_entry(char *string) {
    entry_t *entry = malloc(sizeof(entry_t));

    entry->list = create_list(&record_compare_function, &record_free_function);

    entry->string = malloc(strlen(string) + sizeof(char));
    strcpy(entry->string, string);

    return entry;
}

void free_entry(entry_t *e) {
    free_list(e->list);
    free(e->string);
    free(e);
}