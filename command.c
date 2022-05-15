#include "command.h"

// insereaza un nod in tr-o lista si returneaza operatia atomica corespunzatoare
Command insert(List **list, char value) {
    List *new_node = malloc(sizeof(List));
    new_node->value = value;
    new_node->next = *list;
    new_node->prev = (*list)->prev;
    if (new_node->prev != NULL)
        new_node->prev->next = new_node;
    if (new_node->next != NULL)
        new_node->next->prev = new_node;
    Command command;
    command.character = value;
    command.index = get_index(new_node);
    command.is_inserted = 1;
    return command;
}

// sterge un nod din lista si returneaza operatia atomica corespunzatoare
Command erase(List **list) {
    List *aux_node = *list,
         *left_node = (*list)->prev,
         *right_node = (*list)->next;
    assert(right_node != NULL);
    Command command;
    command.character = (*list)->value;
    command.index = get_index(*list);
    command.is_inserted = 0;
    if (left_node != NULL)
        left_node->next = right_node;
    right_node->prev = left_node;
    *list = right_node;
    free(aux_node);
    return command;
}

// implementeaza un vector de dimensiune variabila de comenzi (pe principiul de la std::vector din C++)
Command* push_back(Command *command_list, Command command, int *size, int *nr_commands) {
    if ((*nr_commands)++ >= *size) {
        if (*size == 0) {
            *size = 1;
            command_list = malloc((*size) * sizeof(Command));
        } else {
            *size *= 2;
            command_list = realloc(command_list, (*size) * sizeof(Command));
        }
    }
    command_list[(*nr_commands) - 1] = command;
    return command_list;
}