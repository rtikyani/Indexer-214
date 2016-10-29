#include <stdio.h>
#include <stdlib.h>
#include "list.h"

list_t *create_list(compare_function_t *compare_function, free_function_t *free_function) {

    list_t *list = malloc(sizeof(list_t));

    if (list == 0) {
        return 0;
    }

    list->compare_function = compare_function;
    list->free_function = free_function;
    list->start = 0;

    return list;
}

void free_list(list_t *list) {
    list_item_t *index = list->start;

    while (index != 0) {
        list_item_t *next = index->next_item;
        list->free_function(index->data);
        free(index);

        index = next;
    }

    free(list);
}

list_item_t *create_list_item(void *data, list_item_t *next_item) {
    list_item_t *element = malloc(sizeof(list_item_t));

    if (element == 0) {
        return 0;
    }

    element->data = data;
    element->next_item = next_item;
    return element;
}

static void insert_after(list_item_t *index, list_item_t *element) {
    list_item_t *tmp = index->next_item;
    element->next_item = tmp;
    index->next_item = element;
}

static int insert_before(list_t *list, list_item_t *index, list_item_t *element) {

    if (index == list->start) {
        element->next_item = list->start;
        list->start = element;

    } else {

        list_item_t *tmp = list->start;

        while (tmp != 0) {
            if (tmp->next_item == index) {
                insert_after(tmp, element);
                return 1;
            }
            tmp = tmp->next_item;
        }

    }

    return 0;
}

int add_item(list_t *list, void *item) {
    if (list->start == 0) {

        list->start = create_list_item(item, 0);

    } else {

        list_item_t *index = list->start;

        while (1) {
            int result = list->compare_function(item, index->data);

            if(result == 1 || result == 0) {
                insert_before(list, index, create_list_item(item, 0));
                return 1;
            }

            if (index->next_item == 0) {
                insert_after(index, create_list_item(item, 0));
                return 1;
            }

            index = index->next_item;
        }

    }
    return 0;
}

iterator_t *create_iterator(list_t *list) {
    iterator_t *iterator = malloc(sizeof(iterator_t));
    iterator->index = list->start;
    return iterator;
}

void free_iterator(iterator_t *iterator) {
    free(iterator);
}

void *get(iterator_t *iterator) {

    if (iterator->index != 0) {
        return iterator->index->data;
    }

    return 0;
}

void *next(iterator_t *iterator) {
    list_item_t *next_item = iterator->index->next_item;

    if (next_item != 0) {
        iterator->index = next_item;
        return next_item->data;
    }

    return 0;
}