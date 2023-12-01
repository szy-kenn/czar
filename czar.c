#include "lexer/lexer.h"
#include <ctype.h>
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
        dest[currentPosition - (dotPosition + 1)] = tolower(currentChar);
        currentPosition++;
    }

    // add a string terminator to mark the end of string
    dest[currentPosition - (dotPosition + 1)] = '\0';
}

int save_tokens(Token *token_array, int arr_length) {
    // tokens_print(token_array, arr_length);
    FILE *fp = fopen("czar-lexer.txt", "w");

    if (fp == NULL) {
        return -1;
    }

    int spaces_length = (15 - strlen("TOKEN TYPE"));
    char *spaces = malloc(spaces_length);
    memset(spaces, ' ', spaces_length);
    spaces[spaces_length] = '\0';
    fprintf(fp, "TOKEN TYPE%sVALUE\n", spaces);
    free(spaces);

    fprintf(fp, "====================\n");

    for (int i = 0; i < arr_length; i++) {
        int spaces_length = (15 - strlen(token_array[i].name));
        char *spaces = malloc(spaces_length);
        memset(spaces, ' ', spaces_length);
        spaces[spaces_length] = '\0';

        fprintf(fp, "%s%s%s\n", token_array[i].name, spaces, token_array[i].value);
        free(spaces);
    }
    fclose(fp);
    return 0;
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
        printf("\033[0;31mError:\033[0;37m No file name has been passed.\n");
        printf("\033[0;32mUsage:\033[0;37m czar \033[0;93m<file-name>\033[0;37m.cz");
        return -1;
    }

    // get the file name from the passed argument
    char fileName[strcspn(argv[1], "\0")];
    strcpy(fileName, argv[1]);

    // check if the file extension is valid (.cz)
    char fileExt[REG_BUFFER];
    get_file_ext(fileExt, fileName);

    if (strcmp(fileExt, "cz") != 0) {
        printf("\033[0;31mError:\033[0;37m Invalid file type. Please input a `.cz` file.");
        return -1;
    }

    // open the file
    FILE *fp = fopen(fileName, "r");

    if (fp == NULL) {
        printf("\033[0;31mError:\033[0;37m Could not open \033[0;93m`%s`\033[0;37m", fileName);
        return -1;
    }

    // ==================== start lexical analysis here ==================== //

    // create array of tokens (for symbol table)
    Token *token_array = malloc(sizeof(Token) * MAX_BUFFER);
    int token_count;
    token_count = start_tokenization(fp, token_array);

    if (save_tokens(token_array, token_count) < 0) {
        printf("\033[0;31mTask failed. A problem has occured while opening a file.\033[0;37m");
    } else {
        printf("\033[0;32mSuccess:\033[0;37m Tokenization output saved in "
               "\033[0;33m`czar-lexer.txt`\033[0;37m");
    }

    fclose(fp);
    tokens_free(token_array, token_count);
    return 0;
}