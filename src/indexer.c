#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include "indexer.h"
#include "tokenizer.h"
#include "entry.h"
#include "util.h"

static entry_record_t *get_entry_record(entry_t *entry, char *path) {

    iterator_t *it = create_iterator(entry->list);
    entry_record_t *rec = get(it);

    while (rec != 0) {

        if (strcmp(path, rec->path) == 0) {
            return rec;
        }

        rec = next(it);

    }

    return 0;
}

static entry_t *get_indexer_entry(indexer_t *i, char *t) {
    iterator_t *iterator = create_iterator(i->items);
    entry_t *current = get(iterator);

    while (current != 0) {

        if (strcmp(t, current->string) == 0) {
            return current;
        }

        current = next(iterator);
    }

    return 0;
}

static int entry_compare_function(void *first, void *second) {
    entry_t *first_entry = first;
    entry_t *second_entry = second;

    return strcmp(first_entry->string, second_entry->string) > 0 ? -1 : 1;
}

static void entry_destroy_function(void *object) {
    free_entry((entry_t *) object);
}

indexer_t *create_indexer() {
    indexer_t *idx = malloc(sizeof(indexer_t));
    idx->items = create_list(&entry_compare_function, &entry_destroy_function);
    return idx;
}

void free_indexer(indexer_t *idx) {
    free_list(idx->items);
    free(idx);
}


static void handle_token(indexer_t *idx, char *path, char *string) {
    entry_t *entry = get_indexer_entry(idx, string);

    if (entry == 0) {
        entry = create_entry(string);
        add_item(idx->items, entry);
    }

    entry_record_t *rec = get_entry_record(entry, path);

    if (rec == 0) {
        rec = create_entry_record(path);
        add_item(entry->list, rec);
    }

    rec->amount++;
}

static int parse_file(indexer_t *indexer, char *file_path) {
    char *file_data = get_file_contents(file_path);

    if (file_data == 0) {
        return 0;
    }

    list_t *token_list = get_tokens(file_data);
    iterator_t *iterator = create_iterator(token_list);

    if (iterator != 0) {

        char *token = get(iterator);

        while (token != 0) {
            handle_token(indexer, file_path, token);
            token = next(iterator);
        }

        free_iterator(iterator);
    }

    free_list(token_list);
    free(file_data);

    return 1;
}

static int traverse(indexer_t *idx, char *path) {
    DIR *d = opendir(path);

    if (d != 0) {

        struct dirent *file;

        while ((file = readdir(d)) != 0) {

            char *name = file->d_name;

            if (strcmp(name, "..") == 0 || strcmp(name, ".") == 0) {
                continue;
            }

            size_t size = strlen(path) + strlen(name) + (2 * sizeof(char));

            char *file_path = malloc(size);
            strcpy(file_path, path);
            strcat(file_path, "/");
            strcat(file_path, name);

            if (file->d_type == DT_REG) {
                parse_file(idx, file_path);
            } else if (file->d_type == DT_DIR) {
                traverse(idx, file_path);
            }

            free(file_path);
        }

        closedir(d);

        return 1;
    } else {
        return 0;
    }
}

int run_indexer(indexer_t *idx, char *path) {
    if (!traverse(idx, path) && !parse_file(idx, path)) {
        return 0;
    }
    return 1;
}
