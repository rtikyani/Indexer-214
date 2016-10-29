#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "indexer.h"
#include "entry.h"

int main(int argc, char **argv) {

    if (argc != 3) {
        fprintf(stderr, "Wrong number of arguments.");
        return 1;
    } else if (strcmp(argv[1], argv[2]) == 0) {
        fprintf(stderr, "Both input files are the same!\n");
        return 1;
    }

    char *new_file_path = argv[1];
    char *input_path = argv[2];

    FILE *new_file;

    if (access(new_file_path, F_OK) != -1) {

        printf("That file already exists! Type '1' to overwrite or '2' to cancel.\n");
        fflush(stdout);
        int option;
        scanf("%d", &option);

        if (option == 2) {
            return 0;
        }

        new_file = fopen(new_file_path, "w");
    } else {
        new_file = fopen(new_file_path, "a+");
    }

    indexer_t *indexer = create_indexer();
    int successful = run_indexer(indexer, input_path);

    if(!successful) {
        fprintf(stderr, "An error occurred while parsing!\n");
    } else {
        iterator_t *items = create_iterator(indexer->items);
        entry_t *current = get(items);

        fprintf(new_file, "{\"list\": [\n");

        while (current != 0) {

            fprintf(new_file, "\t{\"%s\": [\n", current->string);

            iterator_t *record_iterator = create_iterator(current->list);
            entry_record_t *record = get(record_iterator);

            while (record != 0) {
                fprintf(new_file, "\t\t{\"%s\" : %i}", record->path, record->amount);
                record = next(record_iterator);

                if (record != 0) {
                    fprintf(new_file, ",\n");
                }
            }

            fprintf(new_file, "\n\t]}\n");
            free_iterator(record_iterator);

            current = next(items);
        }

        fprintf(new_file, "]}\n");
        free_iterator(items);

    }

    fclose(new_file);
    free_indexer(indexer);

    return successful ? 0 : 1;
}