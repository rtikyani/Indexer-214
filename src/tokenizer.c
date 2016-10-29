#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "tokenizer.h"

static int valid_char(char c) {
    return (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9');
}

static int compare_function(void *first, void *second) {
    return 1;
}

static void free_function(void *str) {
    free(str);
}

static char *create_token(char *start, char *index) {
    size_t length = index - start;

    if (length == 0) {
        return 0;
    }

    char *result = malloc(length + sizeof(char));
    memcpy(result, start, length);
    result[length] = '\0';

    return result;
}

list_t *get_tokens(char *s) {
    list_t *list = create_list(&compare_function, &free_function);

    if (list == 0) {
        return 0;
    }

    char *start = s;
    char *end_index = s + strlen(s);
    char *idx = s;

    while (idx != end_index) {
        if (!valid_char(*idx)) {

            char *token = create_token(start, idx);
            if (token != 0) {
                add_item(list, token);
            }

            idx += sizeof(char);
            start = idx;

        } else {

            idx += sizeof(char);

            if (idx == end_index) {

                char *token = create_token(start, idx);

                if (token != 0) {
                    add_item(list, token);
                }

            }
        }
    }

    return list;
}