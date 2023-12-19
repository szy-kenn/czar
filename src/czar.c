#include "czar-state-machine/czar-state-machine.h"
#include "file_handler/file_handler.h"
#include "lexer/lexer.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_BUFFER 100 // max buffer size for memory allocation
#define REG_BUFFER 30 // regular buffer size for memory allocation

// int save_tokens(Token *token_array, int arr_length, const char *outputFile) {
//     FILE *fp = fopen(outputFile, "w");

//     if (fp == NULL) {
//         return -1;
//     }

//     int spaces_length = (15 - strlen("TOKEN TYPE"));
//     char *init_spaces = (char *)malloc(sizeof(char) * spaces_length + 1);

//     for (int i = 0; i < spaces_length; i++) {
//         init_spaces[i] = ' ';
//     }
//     init_spaces[spaces_length] = '\0';

//     fprintf(fp, "TOKEN TYPE%sLEXEME\n", init_spaces);
//     free(init_spaces);

//     fprintf(fp, "====================\n");

//     for (int i = 0; i < arr_length; i++) {
//         int spaces_length = (15 - strlen(token_array[i].name));
//         char *spaces = (char *)malloc(sizeof(char) * spaces_length + 1);
//         for (int i = 0; i < spaces_length; i++) {
//             spaces[i] = ' ';
//         }
//         spaces[spaces_length] = '\0';

//         fprintf(fp, "%s%s%s\n", token_array[i].name, spaces,
//         token_array[i].value); free(spaces);
//     }
//     fclose(fp);
//     return 0;
// }

/**
 * argc is the length of argv (command-line arguments + command)
 * argv is an array of character pointers of all arguments passed after the
 *command argv[0] is the actual command invoked (e.g. ./czar), argv[1], argv[2],
 *... argv[argc-1] are all the strings passed as arguments after the command
 * (e.g. "test.cz" in ./czar test.cz)
 **/
int main(int argc, char **argv) {

    // check if one argument has been passed
    if (argc == 1) {
        printf("\033[0;31mError:\033[0;37m No czar file has been passed.\n");
        printf("\033[0;32mUsage:\033[0;37m czar "
               "\033[0;93m<file-name>\033[0;37m.cz "
               "\033[0;93m<file-name>\033[0;37m.txt");
        return -1;
    }

    if (argc == 2) {
        printf("\033[0;31mError:\033[0;37m No output file name has been "
               "passed.\n");
        printf("\033[0;32mUsage:\033[0;37m czar "
               "\033[0;93m<file-name>\033[0;37m.cz "
               "\033[0;93m<file-name>\033[0;37m.txt");
        return -1;
    }

    clock_t begin = clock();

    // get the czar file from the passed argument
    char czar_file[strcspn(argv[1], "\0")];
    strcpy(czar_file, argv[1]);

    // get the output txt file from the passed argument
    char output_file[strcspn(argv[2], "\0")];
    strcpy(output_file, argv[2]);

    if (!file_ext_cmp(czar_file, "cz")) {
        printf("\033[0;31mError:\033[0;37m Invalid file type. Please input a "
               "`.cz` file.");
        return -1;
    }

    char *source_code = get_file_content(czar_file);

    // ==================== start lexical analysis here ==================== //
    StateMachine *czar_state_machine = czar_state_machine_init();
    StateMachine *indentation_state_machine = indentation_state_machine_init();
    lexer_initialize(source_code, czar_state_machine,
                     indentation_state_machine);
    int token_count = lexer_start(true);

    if (token_count > 0) {
        int tokens_save_res = tokens_save(output_file);
        if (tokens_save_res < 0) {
            printf("\033[0;31mTask failed. A problem has occured while "
                   "opening a file.\033[0;37m");
        } else {
            printf("\033[0;32mDone:\033[0;37m Tokenization output saved in "
                   "\033[0;33m`%s`\033[0;37m",
                   output_file);
        }
    }
    // else if (token_count == 0) {
    //     printf("\033[0;32mDone: \033[0;37mNo tokens created.\n");
    // } else {
    //     printf("ERROR\n");
    // }

    lexer_free();

    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("\nExecution Time: %fms", time_spent);
    return 0;
}