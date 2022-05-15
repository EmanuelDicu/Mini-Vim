#ifndef OPERATIONS_H_
#define OPERATIONS_H_

#include "list.h"
#include "stack.h"
#include "command.h"
#include "utilityfile.h"

#include <stdlib.h>
#include <string.h>

// antetele tuturor operatiilor complexe
void switch_mode(int *mode);
void insert_text(List **list, char *text, Stack **undo_stack, Stack **redo_stack);
void delete_chars(List **list, char *text, Stack **undo_stack, Stack **redo_stack);
void goto_line(List **list, char *text, Stack **undo_stack, Stack **redo_stack);
void goto_character(List **list, char *text, Stack **undo_stack, Stack **redo_stack);
void save(Stack **undo_stack, Stack **redo_stack);
void backspace(List **list, Stack **undo_stack, Stack **redo_stack);
void delete_line(List **list, char *text, Stack **undo_stack, Stack **redo_stack);
void replace_word(List **list, char *text, Stack **undo_stack, Stack **redo_stack);
void replace_all(List **list, char *text, Stack **undo_stack, Stack **redo_stack);
void delete_word(List **list, char *text, Stack **undo_stack, Stack **redo_stack);
void delete_all(List **list, char *text, Stack **undo_stack, Stack **redo_stack);
void undo(List **list, Stack **undo_stack, Stack **redo_stack);
void redo(List **list, Stack **undo_stack, Stack **redo_stack);
void quit(List **list, Stack **undo_stack, Stack **redo_stack);

#endif