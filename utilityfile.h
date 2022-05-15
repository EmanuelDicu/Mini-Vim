#ifndef UTILITY_H_
#define UTILITY_H_

#include "list.h"
#include "stack.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// antete pentru operatiile utilitare
char* find_word(char **text);
int match(List *list, char *text);
void print_list(FILE *out_file, List *list);
int get_number(char **text);

#endif