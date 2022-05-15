#include "list.h"

// gaseste inceputul unei liste
List* get_beginning(List *list) {
    assert(list != NULL);
    while (list->prev != NULL)
        list = list->prev;
    return list;
}

// gaseste indexul unui nod oarecare in lista
int get_index(List *list) {
    int index = 0;
    while (list != NULL) {
        ++index;
        list = list->prev;
    }
    return index;
}

// returneaza pointer la nodul cu un anumit index
List* goto_index(List *list, int index) {
    list = get_beginning(list);
    int i;
    for (i = 1; i < index; ++i) {
        list = list->next;
        assert(list != NULL);
    }
    return list;
}

// creaza o lista goala
List* empty_list() {
    List *new_node = malloc(sizeof(List));
    new_node->prev = new_node->next = NULL;
    new_node->value = -1;
    return new_node;
}

// gaseste nodul de dupa al 'line'-lea newline
List* line_beginning(List *list, int line) {
    if (line == -1) {
        while (list->prev != NULL && list->prev->value != '\n')
            list = list->prev;
    } else {
        list = get_beginning(list);
        int current_line = 1;
        while (current_line < line && list->value != -1) {
            if (list->value == '\n')
                ++current_line;
            list = list->next;
        }
        assert(current_line == line);
    }
    return list;
}

// muta cursorul la nodul cu 'column' pozitii mai la dreapta de dupa nodul cu al
// 'line'-lea newline din lista
List* move_cursor(List *list, int line, int column) {
    list = line_beginning(list, line);
    int i;
    for (i = 1; i < column && list->value != '\n' && list->value != -1; ++i) 
        list = list->next;
    assert(list->value != '\n' && list->value != -1);
    return list;
}

// goleste o lista
void clear_list(List *list) {
    list = get_beginning(list);
    while (list != NULL) {
        List *aux_node = list;
        list = list->next;
        free(aux_node);
    }
}
