#include "utils.h"
#include <stdio.h>

void print_error(char *error, char *message, int line) {
    printf("\033[0;31m%s\033[0;37m: %s at line %d", error, message, line);
}