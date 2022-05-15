#include "stack.h"

// adauga un nod in varful stivei
Stack* push(Stack **stack, Command *command, 
            int nr_commands, int cursor_index_before, int cursor_index_after) {
    Stack *new_node = malloc(sizeof(Stack));
    new_node->command = command;
    new_node->nr_commands = nr_commands;
    new_node->prev = *stack;
    new_node->cursor_index_before = cursor_index_before;
    new_node->cursor_index_after = cursor_index_after;
    *stack = new_node;
    return *stack;
}

// adauga un nod deja creat in varful stivei
Stack* push2(Stack **stack, Stack *new_node) {
    new_node->prev = *stack;
    *stack = new_node;
    return *stack;
}

// sterge varful stivei
Stack* pop(Stack **stack) {
    assert(*stack != NULL);
    if ((*stack)->command != NULL) {
        free((*stack)->command);
        (*stack)->command = NULL;
    }
    Stack *aux_node = *stack;
    *stack = aux_node->prev;
    free(aux_node);
    aux_node = NULL;
    return *stack;
}

// returneaza pointer la varful stivei
Stack* top(Stack *stack) {
    return stack;
}

// verifica daca stiva este goala
int is_empty(Stack *stack) {
    return (stack == NULL);
}

// goleste stiva
Stack* clear_stack(Stack **stack) {
    while (!is_empty(*stack))
        pop(stack);
    return *stack;
}

// muta varful unei stive pe alta stiva
Stack *move_top(Stack **stack1, Stack **stack2) {
    if (*stack1 == NULL)
        return *stack1;
    Stack *node = *stack1;
    *stack1 = node->prev;
    node->prev = *stack2;
    *stack2 = node;
    return *stack1;
}