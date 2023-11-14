#include <stdio.h>
#include <string.h>

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("usage: czar <file-name>.cz");
        return -1;
    }

    int dotPosition = strcspn(argv[1], ".");
    char fileExt[30];

    char currentChar = argv[1][dotPosition + 1];
    char currentPosition = dotPosition + 1;

    while (currentChar != '\0') {
        currentChar = argv[1][currentPosition];
        fileExt[currentPosition - (dotPosition + 1)] = currentChar;
        currentPosition++;
    }

    fileExt[currentPosition - (dotPosition + 1)] = '\0';

    if (strcmp(fileExt, "cz") != 0) {
        printf("Invalid file type. Please input a `.cz` file.");
        return -1;
    }

    printf("%s", fileExt);
    return 0;
}