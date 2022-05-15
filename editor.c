#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "list.h"
#include "stack.h"
#include "command.h"
#include "utilityfile.h"
#include "operations.h"

int main(int argc, const char **argv) {
    // citesc si verific daca s-au deschis fisierele
    FILE *in_file = fopen("editor.in", "r"),
         *out_file = fopen("editor.out", "w");
    assert(in_file != NULL && out_file != NULL);

    // initializez stivele, lista si sirurile de caractere in care
    // o sa citesc inputul
    Stack *undo_stack = NULL, 
          *redo_stack = NULL;
    List *list = empty_list();
    char buffer[100];
    int mode = 0;
    char text[10000];
    int ok = 1;

    // implementarea propriuzisa a algoritmului
    while (ok) {
        if (mode == 0) { // ma aflu in modul insert
            memset(text, 0, sizeof(text));
            while (ok) { // citesc tot paragraful
                ok = (fgets(buffer, sizeof(buffer), in_file) != NULL);
                if (!ok || !strncmp(buffer, "::i", 3))
                    break;
                strcat(text, buffer);
            }
            insert_text(&list, text, &undo_stack, &redo_stack); // inserez paragraful
            if (ok)
                switch_mode(&mode);
        } else {
            ok = (fgets(buffer, sizeof(buffer), in_file) != NULL);

            // aplic operatia corespunzatoare
            if (!strncmp(buffer, "q", 1)) {
                quit(&list, &undo_stack, &redo_stack);
                break;
            } else if (!strncmp(buffer, "::i", 3))
                switch_mode(&mode);
            else if (!strncmp(buffer, "s", 1)) 
                save(&undo_stack, &redo_stack);
            else if (!strncmp(buffer, "b", 1)) 
                backspace(&list, &undo_stack, &redo_stack);
            else if (!strncmp(buffer, "gl", 2))
                goto_line(&list, buffer, &undo_stack, &redo_stack);
            else if (!strncmp(buffer, "gc", 2))
                goto_character(&list, buffer, &undo_stack, &redo_stack);
            else if (!strncmp(buffer, "dl", 2))
                delete_line(&list, buffer, &undo_stack, &redo_stack);
            else if (!strncmp(buffer, "re", 2))
                replace_word(&list, buffer, &undo_stack, &redo_stack);
            else if (!strncmp(buffer, "ra", 2))
                replace_all(&list, buffer, &undo_stack, &redo_stack);
            else if (!strncmp(buffer, "dw", 2))
                delete_word(&list, buffer, &undo_stack, &redo_stack);
            else if (!strncmp(buffer, "da", 2))
                delete_all(&list, buffer, &undo_stack, &redo_stack);
            else if (!strncmp(buffer, "u", 1))
                undo(&list, &undo_stack, &redo_stack);
            else if (!strncmp(buffer, "r", 1))
                redo(&list, &undo_stack, &redo_stack);
            else if (!strncmp(buffer, "d", 1))
                delete_chars(&list, buffer, &undo_stack, &redo_stack);
        }
        // print_list(NULL, list);
    }
    // afisez raspunsul si curat lista si stivele, apoi inchid fisierele
    print_list(out_file, list);
    clear_list(list);
    clear_stack(&undo_stack);
    clear_stack(&redo_stack);
    fclose(in_file);
    fclose(out_file);
    return 0;
}