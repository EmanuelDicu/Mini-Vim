#ifndef COMMAND_H_
#define COMMAND_H_

#include "list.h"

#include <stdlib.h>

// structura unei comenzi
typedef struct command {
    int is_inserted; // 1/0 daca nodul a fost inserat sau sters
    int index; // pozitia nodului in lista
    char character; // caracterul din nod
} Command;

// antete pentru operatiile cu comenzi
Command insert(List **list, char value);
Command erase(List **list);
Command* push_back(Command *command_list, Command command, int *size, int *nr_commands);

#endif