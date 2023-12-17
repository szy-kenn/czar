#include "charset.h"

char *charset_create(char *new_char_set) {
    char *dest = malloc(strlen(new_char_set) + 1);
    strcpy(dest, new_char_set);
    return dest;
}

char *charset_includes(char *char_set, char *includes) {
    char *dest = malloc(strlen(char_set) + strlen(includes) + 1);

    strcpy(dest, char_set);
    int count = strlen(char_set);

    for (int i = 0; i < strlen(includes); i++) {
        if (strchr(char_set, includes[i]) == NULL) {
            dest[count++] = includes[i];
        }
    }

    dest[count] = '\0';
    return dest;
}

char *charset_excludes(char *char_set, char *excludes) {
    char *dest = malloc(strlen(char_set) + 1);

    int count = 0;
    char *cur = char_set;

    while (*cur != '\0') {
        bool is_excluded = false;
        for (int i = 0; i < strlen(excludes); i++) {
            if (*cur == excludes[i]) {
                is_excluded = true;
                break;
            }
        }

        if (!is_excluded) {
            dest[count++] = *cur;
        }

        *cur++;
    }

    dest[count] = '\0';
    return dest;
}