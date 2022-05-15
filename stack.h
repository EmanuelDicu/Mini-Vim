#ifndef STACK_H_
#define STACK_H_

#include "command.h"

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

// structura unui nod de stiva (operatie complexa)
typedef struct stack_node {
    int cursor_index_before; // pozitia cursorului inaintea aplicarii operatiei
    int cursor_index_after; // pozitia cursorului dupa aplicarea operatiei
    Command *command; // vectorul de comenzi (operatii 'atomice') aplicate in cadrul operatiei curente
    int nr_commands; // numarul de operatii atomice aplicate
    struct stack_node *prev; // elementul imediat de sub nod
} Stack;

// antete pentru operatiile pe stiva
Stack* push(Stack **stack, Command *command, 
            int nr_commands, int cursor_index_before, int cursor_index_after);
Stack* push2(Stack **stack, Stack *new_node);
Stack* pop(Stack **stack);
Stack* top(Stack *stack);
Stack* clear_stack(Stack **stack);
Stack *move_top(Stack **stack1, Stack **stack2);

int is_empty(Stack *stack);

#endif