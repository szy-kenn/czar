#include "lexer/lexer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BUFFER 100 // max buffer size for memory allocation
#define REG_BUFFER 30 // regular buffer size for memory allocation

/**
 * Getting the file extension from the passed file name (src)
 * and storing it in dest
 */
void get_file_ext(char *dest, const char *src) {
    int dotPosition = strcspn(src, "."); // get the position of the dot first
    char currentPosition = dotPosition + 1; // start at the next character after the dot
    char currentChar;

    // while it hasn't reached the end of string yet, copy the contents from src to dest
    while ((currentChar = src[currentPosition]) != '\0') {
        dest[currentPosition - (dotPosition + 1)] = currentChar;
        currentPosition++;
    }

    // add a string terminator to mark the end of string
    dest[currentPosition - (dotPosition + 1)] = '\0';
}

/**
 * argc is the length of argv (command-line arguments + command)
 * argv is an array of character pointers of all arguments passed after the command
 * argv[0] is the actual command invoked (e.g. ./czar),
 * argv[1], argv[2], ... argv[argc-1] are all the strings passed as arguments after the command
 * (e.g. "test.cz" in ./czar test.cz)
 **/
int main(int argc, char **argv) {

    // check if one argument has been passed
    if (argc != 2) {
        printf("Error: No file name has been passed.\nUsage: czar <file-name>.cz");
        return -1;
    }

    // get the file name from the passed argument
    char fileName[strcspn(argv[1], "\0")];
    strcpy(fileName, argv[1]);

    // check if the file extension is valid (.cz)
    char fileExt[REG_BUFFER];
    get_file_ext(fileExt, fileName);

    if (strcmp(fileExt, "cz") != 0) {
        printf("Error: Invalid file type. Please input a `.cz` file.");
        return -1;
    }

    // open the file
    FILE *fp = fopen(fileName, "r");

    if (fp == NULL) {
        printf("Error: Could not open %s", fileName);
        return -1;
    }

    // ==================== start lexical analysis here ==================== //

    // create array of tokens (for symbol table)
    Token *tokenArray = malloc(sizeof(Token) * MAX_BUFFER);
    start_tokenization(fp, tokenArray);

    fclose(fp);
    free(tokenArray);
    return 0;
}