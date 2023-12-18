#define STRING_SET                                                             \
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789\n\t\r"     \
    " !@#$%^&*()_+-={[]}:;\"'|\\?/>.<,~`"

#define CHARACTER_SET                                                          \
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789\n\t\r"     \
    " +-*/=><'\"?%_()[]:,|^@"
#define ALPHABETS "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"
#define UPPERCASE "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
#define LOWERCASE "abcdefghijklmnopqrstuvwxyz"
#define IDENTIFIER_SET                                                         \
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_"
#define DIGITS "0123456789"
#define SPACES "\n\t\r "
#define SYMBOLS "+-*/=><'\"?%_()[]:,|^@"
#define RELATIONAL "=<>"
#define ARITHMETIC "+-*/%^|"

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

char *charset_create(char *new_char_set);
char *charset_includes(char *char_set, char *includes);
char *charset_excludes(char *char_set, char *excludes);