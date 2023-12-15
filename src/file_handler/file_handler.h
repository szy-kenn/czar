#ifndef _CZAR_FILE_HANDLER_H_
#define _CZAR_FILE_HANDLER_H_

#include "../utils/utils.h"

bool file_ext_cmp(const char *file_name, const char *ext);
size_t get_file_size(const char *czar_file);
char *get_file_content(char *czar_file);

#endif