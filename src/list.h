#ifndef _SORTED_LIST_H_
#define _SORTED_LIST_H_

#include <stdint.h>

typedef int compare_function_t(void *, void *);

typedef void free_function_t(void *);

typedef struct list_item {
    struct list_item *next_item;
    void *data;
} list_item_t;

typedef struct list {
    compare_function_t *compare_function;
    free_function_t *free_function;
    list_item_t *start;
} list_t;

list_t *create_list(compare_function_t *, free_function_t *);

void free_list(list_t *);

list_item_t *create_list_item(void *, list_item_t *);

int add_item(list_t *, void *);

typedef struct iterator {
    list_item_t *index;
} iterator_t;

iterator_t *create_iterator(list_t *);

void free_iterator(iterator_t *);

void *get(iterator_t *);

void *next(iterator_t *);

#endif