#include "../utils/utils.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* compare the passed file's extension to the passed string */
bool file_ext_cmp(const char *path, const char *ext) {

    /* get the last idx value of the two strings (file name and ext to be
     * compared) */
    int path_idx = strlen(path) - 1;
    int ext_idx = strlen(ext) - 1;

    /* compare each characters of file name and ext, starting at the end */
    for (; path_idx >= 0 && ext_idx >= 0; path_idx--, ext_idx--) {
        if (tolower(path[path_idx]) != tolower(ext[ext_idx])) {
            return false;
        }
    }

    return path_idx >= 0 && path[path_idx] == '.';
}

/* gets the size of the passed file name */
size_t get_file_size(const char *path) {
    FILE *fp = fopen(path, "rb");

    if (fp == NULL) {
        printf("CANT OPEN FILE!");
        exit(-1);
    }

    fseek(fp, 0, SEEK_END);
    size_t file_size = ftell(fp);
    fclose(fp);

    return file_size;
}

char *get_file_content(char *czar_file) {

    size_t file_size = get_file_size(czar_file);
    char *dest = (char *)malloc(file_size + 1);

    if (dest == NULL) {
        printf("CANNOT ALLOCATE MEMORY");
        exit(-2);
    }

    FILE *fp = fopen(czar_file, "rb");

    if (fp == NULL) {
        printf("\033[0;31mError:\033[0;37m Could not open "
               "\033[0;93m`%s`\033[0;37m",
               czar_file);
        exit(-1);
    }

    char chars_read = fread(dest, sizeof(char), file_size, fp);
    dest[chars_read] = '\0';
    fclose(fp);
    return dest;
}