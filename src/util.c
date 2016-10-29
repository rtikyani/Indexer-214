#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

static int downcase(int c) {
    if (c >= 'A' && c <= 'Z') {
        c -= 'A';
        c += 'a';
    }
    return c;
}

char *get_file_contents(char *path) {
    FILE *file = fopen(path, "r");

    if (file != 0) {

        struct stat stat;

        if (fstat(fileno(file), &stat) == 0) {

            if (stat.st_size > 0) {

                char *contents = malloc((size_t) stat.st_size + sizeof(char));
                char *idx = contents;

                int chr;
                while ((chr = fgetc(file)) != EOF) {
                    sprintf(idx, "%c", downcase(chr));
                    idx += sizeof(char);
                }

                sprintf(contents + stat.st_size, "%c", '\0');
                fclose(file);

                return contents;
            } else {
                fclose(file);

                char *contents = malloc(sizeof(char));
                *contents = '\0';
                return contents;
            }

        }

    }

    return 0;
}