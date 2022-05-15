#include "utilityfile.h"

// parseaza un cuvant intr-un sir text
char* find_word(char **text) {
    while (**text == ' ')
        ++(*text);
    char *left_it = *text,
         *right_it = left_it;
    int len = 0;
    while (*right_it != ' ' && *right_it != 0 && *right_it != '\n') {
        ++len;
        ++right_it;
    }
    char *return_string = calloc(len + 1, sizeof(char));
    memcpy(return_string, left_it, len * sizeof(char));
    *text = right_it;
    return return_string;
}

// face 'pattern-matching' intre nodurile consecutive dintr-o lista si un sir de caractere
int match(List *list, char *text) {
    int len = strlen(text);
    int i;
    for (i = 0; i < len; ++i, list = list->next) 
        if (list == NULL || list->value != text[i])
            return 0;
    return 1;
}

// afiseaza intr-un fisier sau la stdout o lista
void print_list(FILE *out_file, List *list) {
    static int val = 0;
    ++val;
    if (out_file == NULL)
        printf("%d:\n", val);
    list = get_beginning(list);
    if (list != NULL) {
        list = get_beginning(list);
        while (list->value != -1) {
            if (out_file == NULL)
                printf("%c", list->value);
            else
                fprintf(out_file, "%c", list->value);
            list = list->next;
        }
    }
    if (out_file == NULL)
        printf("\n\n");
}

// parseaza un numar dintr-un sir text
int get_number(char **text) {
    int number = 0;
    while ((**text !=  0) && (**text < '0' || **text > '9'))
        ++(*text);
    if (**text == 0)
        return -1;
    while ((**text >= '0') && (**text <= '9')) {
        number = number * 10 + (**text - '0');
        ++(*text);
    }
    return number;
}
