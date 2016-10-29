#ifndef _ENTRY_H_
#define _ENTRY_H_

typedef struct entry_record {
    char *path;
    int amount;
} entry_record_t;

entry_record_t *create_entry_record(char *);

void free_entry_record(entry_record_t *);

#endif