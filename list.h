#ifndef LIST_H_
#define LIST_H_

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

// structura unui nod de lista
typedef struct list_node {
    char value; // valoarea din nod
    struct list_node *next, *prev; // pointeri la nodurile din spate si din fata
} List;

// antete pentru operatiile pe lista
List* get_beginning(List *list);
List* goto_index(List *list, int index);
List* empty_list();
List* line_beginning(List *list, int line);
List* move_cursor(List *list, int line, int column);

int get_index(List *list);
void clear_list(List *list);

#endif