#include "operations.h"

// schimb modul din insert in comanda si vice-versa
void switch_mode(int *mode) {
    *mode ^= 1;
}

// inserez un paragraf de text in lista
void insert_text(List **list, char *text, Stack **undo_stack, Stack **redo_stack) {
    int len = strlen(text); // calculez lungimea textului
    if ((*list)->value != -1) { 
        // pentru usurinta implementarii, la finalul fiecarei linii de text se afla un
        // caracter newline, CU EXCEPTIA ultimei linii care este urmata PE LANGA NEWLINE 
        // si de un nod fictiv cu valoare -1. Daca nu am cursorul situat pe acest nod, 
        // trebuie sa il mut cu o pozitie mai la dreapta (si sa elimin acel newline in
        // cazul in care insertul nu se face la finalul textului)
        *list = (*list)->next;
        --len;
    }
    int cursor_index_before = get_index(*list);
    Command *command_list = NULL;
    int size = 0, nr_commands = 0;
    int i;
    for (i = 0; i < len; ++i) // inserez caracter cu caracter si creez lista de comenzi
        command_list = push_back(command_list, insert(list, text[i]), &size, &nr_commands);
    int cursor_index_after = get_index(*list);

    // adaug in stiva de undo noua operatie complexa formata
    push(undo_stack, command_list, nr_commands, cursor_index_before, cursor_index_after);
    clear_stack(redo_stack);
}

// sterge un numar de caractere de la o pozitie fixata
void delete_chars(List **list, char *text, Stack **undo_stack, Stack **redo_stack) {
    int number = get_number(&text);
    if (number == -1)
        number = 1;
    int cursor_index_before = get_index(*list);
    Command *command_list = malloc(number * sizeof(Command));
    int i;
    for (i = 0; i < number; ++i) { // creez lista de comenzi
        if (*list == NULL || (*list)->value == -1) { // daca pot sa sterg in continuare
            number = i + 1;
            command_list = realloc(command_list, number * sizeof(Command));
            break;
        }
        command_list[i] = erase(list);
    }
    int cursor_index_after = get_index(*list);

    // la fel, adaug operatia complexa in lista de undo si curat lista de redo
    push(undo_stack, command_list, number, cursor_index_before, cursor_index_after);
    clear_stack(redo_stack);
}

// muta cursorul la inceputul unei linii
void goto_line(List **list, char *text, Stack **undo_stack, Stack **redo_stack) {
    int cursor_index_before = get_index(*list);
    int line = get_number(&text); 
    *list = line_beginning(*list, line);
    int cursor_index_after = get_index(*list);
    // adaug operatia complexa pe stiva de undo
    push(undo_stack, NULL, 0, cursor_index_before, cursor_index_after);
    clear_stack(redo_stack);
    return;
}

// muta cursorul la o anumita pozitie din fisier
void goto_character(List **list, char *text, Stack **undo_stack, Stack **redo_stack) {
    int cursor_index_before = get_index(*list);
    int column = get_number(&text),
        line = get_number(&text);
    *list = move_cursor(*list, line, column);
    int cursor_index_after = get_index(*list);
    // adaug operatia complexa pe stiva de undo
    push(undo_stack, NULL, 0, cursor_index_before, cursor_index_after);
    clear_stack(redo_stack);
}

// salveaza fisierul
void save(Stack **undo_stack, Stack **redo_stack) {
    // este echivalent cu golirea celor 2 stive
    clear_stack(undo_stack);
    clear_stack(redo_stack);
}

// executa operatia de backspace
void backspace(List **list, Stack **undo_stack, Stack **redo_stack) {
    int cursor_index_before = get_index(*list);
    Command *command_list = malloc(sizeof(Command) * 2);
    if ((*list)->value == -1) { 
        // daca am cursorul pe ultimul nod fictiv, trebuie sa sterg
        // nodul de dinainte de newline (cu 2 pozitii in spate)
        *list = (*list)->prev->prev;
        command_list[0] = erase(list);
    } else 
        command_list[0] = erase(list); // altfel, sterg nodul de sub cursor

    int cursor_index_after = get_index(*list);
    push(undo_stack, command_list, 1, cursor_index_before, cursor_index_after);
    clear_stack(redo_stack);
}

// sterge o linie din fisier
void delete_line(List **list, char *text, Stack **undo_stack, Stack **redo_stack) {
    int line = get_number(&text); // caut argumentul optional
    int cursor_index_before = get_index(*list);
    // ma pozitionez cu un iterator la inceputul liniei si cu celalalt la inceputul liniei urmatoare
    List *left_it = line_beginning(*list, line), 
         *right_it = left_it;
    int nr_deleted = 0;
    while (right_it->value != '\n' && right_it->value != -1) { // caut inceputul liniei urmatoare
        // am grija sa modific pozitia cursorului daca ma urmeaza sa sterg caracterul
        // de sub cursor
        if (*list == right_it)
            *list = (*list)->next;
        right_it = right_it->next;
        ++nr_deleted;
    }
    if (right_it->value == '\n') {
        // daca in urma stergerii liniei ajung la un newline, trebuie sters si acesta
        if (*list == right_it)
            *list = (*list)->next;
        right_it = right_it->next;
        ++nr_deleted;
    }
    Command *command_list = malloc(sizeof(Command) * nr_deleted);
    *list = right_it;
    int i;
    for (i = 0; i < nr_deleted; ++i) // creez lista de comenzi
        command_list[i] = erase(&left_it);
    int cursor_index_after = get_index(*list);

    // adaug in stiva de undo operatia
    push(undo_stack, command_list, nr_deleted, cursor_index_before, cursor_index_after);
    clear_stack(redo_stack);
}

// inlocuieste un cuvant cu alt cuvant
void replace_word(List **list, char *text, Stack **undo_stack, Stack **redo_stack) {
    text += 2;
    // cauta cele 2 argumente ale functiei
    char *old_word = find_word(&text),
         *new_word = find_word(&text);
    // calculeaza lungimea celor 2 cuvinte
    int len_old_word = strlen(old_word),
        len_new_word = strlen(new_word);
    // caut cu un iterator prima aparitie a cuvantului de inlocuit
    List *it = *list;
    while (it->value != -1 && !match(it, old_word))
        it = it->next;
    if (it->value != -1) { // daca am gasit o aparitie
        int cursor_index_before = get_index(*list);
        Command *command_list = malloc((len_old_word + len_new_word) * sizeof(Command));
        int i;
        for (i = 0; i < len_old_word; ++i) { // mai intai sterg nodurile din vechiul cuvant
            if (*list == it)
                *list = it->next;
            command_list[i] = erase(&it);
        }
        for (i = 0; i < len_new_word; ++i) // apoi inserez noile noduri pentru noul cuvant
            command_list[i + len_old_word] = insert(&it, new_word[i]);
        int cursor_index_after = get_index(*list);
        int nr_commands = len_old_word + len_new_word;
        // adaug in stiva de undo operatia complexa
        push(undo_stack, command_list, nr_commands, cursor_index_before, cursor_index_after);
        clear_stack(redo_stack);
    }
    // sterg cuvintele din memorie
    free(old_word);
    free(new_word);
}

// inlocuieste toate aparitiile unui cuvant cu alt cuvant
void replace_all(List **list, char *text, Stack **undo_stack, Stack **redo_stack) {
    text += 2;
    // procesez cele 2 argumente ale functiei (sirurile) respectiv lungimile lor
    char *old_word = find_word(&text),
         *new_word = find_word(&text);
    int len_old_word = strlen(old_word),
        len_new_word = strlen(new_word);
    List *it = get_beginning(*list);
    Command *command_list = NULL;
    int size = 0, nr_commands = 0;
    int cursor_index_before = get_index(*list);
    while (1) { // cat timp caut cuvinte de inlocuit
        while (it->value != -1 && !match(it, old_word))
            it = it->next;
        if (it->value != -1) { // daca am gasit un cuvant de inlocuit
            int i;
            for (i = 0; i < len_old_word; ++i) {
                if (*list == it) // am grija sa mut cursorul inainte sa sterg nodul de sub el
                    *list = it->next;
                // mai intai sterg nodurile corespunzatoare vechiului cuvant
                command_list = push_back(command_list, erase(&it), &size, &nr_commands); 
            }
            // apoi inserez nodurile corespunzatoare noului cuvant
            for (i = 0; i < len_new_word; ++i)
                command_list = push_back(command_list, insert(&it, new_word[i]), &size, &nr_commands);
        } else
            break;
    }
    int cursor_index_after = get_index(*list);
    if (nr_commands) { // redimensionez vectorul de comenzi sa aiba fix dimensiunea numarului de comenzi
        if (size > nr_commands)
            command_list = realloc(command_list, nr_commands * sizeof(Command));
        // inserez in stiva de undo operatia complexa
        push(undo_stack, command_list, nr_commands, cursor_index_before, cursor_index_after);
        clear_stack(redo_stack);
    }
    // curat cuvintele
    free(old_word);
    free(new_word);
}

// sterg prima aparitie a unui cuvant
void delete_word(List **list, char *text, Stack **undo_stack, Stack **redo_stack) {
    text += 2;
    char *word = find_word(&text); // caut cuvantul care trebuie sters
    int len_word = strlen(word);
    List *it = *list;
    while (it->value != -1 && !match(it, word)) // ii caut pozitia
        it = it->next;
    if (it->value != -1) { // daca am gasit o pozitie valida
        int cursor_index_before = get_index(*list);
        Command *command_list = malloc(len_word * sizeof(Command));
        int i;
        for (i = 0; i < len_word; ++i) { // sterg nodurile din lista corespunzatoare cuvantului
            if (*list == it) // am grija sa nu sterg nodul de sub cursor inainte sa il mut
                *list = it->next;
            command_list[i] = erase(&it);
        }
        int cursor_index_after = get_index(*list);
        int nr_commands = len_word;
        // inserez operatia complexa in stiva de undo
        push(undo_stack, command_list, nr_commands, cursor_index_before, cursor_index_after);
        clear_stack(redo_stack);
    }
    free(word);
}

// sterge toate aparitiile unui cuvant din text
void delete_all(List **list, char *text, Stack **undo_stack, Stack **redo_stack) {
    text += 2;
    char *word = find_word(&text); // procesez cuvantul pe care urmeaza sa il sterg
    int len_word = strlen(word);
    List *it = get_beginning(*list);
    Command *command_list = NULL;
    int size = 0, nr_commands = 0;
    int cursor_index_before = get_index(*list);
    while (1) { // cat timp caut cuvantul in lista
        while (it->value != -1 && !match(it, word)) // caut urmatoarea aparitie
            it = it->next;
        if (it->value != -1) { // daca am mai gasit inca o aparitie
            int i;
            for (i = 0; i < len_word; ++i) { // sterg cuvantul din lista
                if (*list == it)
                    *list = it->next;
                command_list = push_back(command_list, erase(&it), &size, &nr_commands);
            }
        } else
            break;
    }
    // am grija sa nu raman cu linii goale in fisier
    it = get_beginning(*list);
    for (; it != NULL && it->value != -1; it = it->next) {
        if (it->value == '\n' && it->next != NULL && it->next->value == '\n') { 
            // daca am gasit o linie goala, o sterg (si eventual deplasez cursorul)
            if (*list == it)
                *list = it->next;
            command_list = push_back(command_list, erase(&it), &size, &nr_commands);
        }
    }

    int cursor_index_after = get_index(*list);
    if (nr_commands) { // redimensionez vectorul de comenzi sa aiba dimensiunea exacta
        if (size > nr_commands)
            command_list = realloc(command_list, nr_commands * sizeof(Command));
        // adaug operatia complexa in stiva de undo
        push(undo_stack, command_list, nr_commands, cursor_index_before, cursor_index_after);
        clear_stack(redo_stack);
    }
    free(word); // sterg cuvantul din memorie
}

// executa operatia de undo
void undo(List **list, Stack **undo_stack, Stack **redo_stack) {
    if (!is_empty(*undo_stack)) { // daca am cel putin o operatie de undo
        Stack *stack_top = top(*undo_stack);
        // aplic in ordine inversa operatiile 'inverse' operatiilor atomice
        int i;
        for (i = stack_top->nr_commands - 1; i >= 0; --i) {
            Command command = stack_top->command[i];
            *list = goto_index(*list, command.index);
            if (command.is_inserted) 
                erase(list); // operatia inversa insertului este erase
            else {
                insert(list, command.character); // si operatia inversa eraseului este insert
                *list = (*list)->prev;
            }
        }
        // repozitionez cursorul unde se afla inainte de operatie
        *list = goto_index(*list, stack_top->cursor_index_before);
        move_top(undo_stack, redo_stack); // mut varful stivei de undo in varful stivei de redo
    }
}

// executa operatia de redo
void redo(List **list, Stack **undo_stack, Stack **redo_stack) {
    if (!is_empty(*redo_stack)) { // daca pot executa cel putin o operatie de redo
        Stack *stack_top = top(*redo_stack);
        // aplic operatiile atomice fix in ordinea in care au fost aplicate initial
        int i;
        for (i = 0; i < stack_top->nr_commands; ++i) { 
            Command command = stack_top->command[i];
            *list = goto_index(*list, command.index);
            if (command.is_inserted) 
                insert(list, command.character);
            else 
                erase(list);
        }
        // repozitionez indexul unde se afla dupa executarea operatiei originale
        *list = goto_index(*list, stack_top->cursor_index_after);
        move_top(redo_stack, undo_stack); // mut la loc varful stivei de redo in varful stivei de undo
    }
}

// operatia de inchidere a programului
void quit(List **list, Stack **undo_stack, Stack **redo_stack) {
    // dau inapoi toate operatiile care au fost executate dar nesalvate
    while (!is_empty(*undo_stack))
        undo(list, undo_stack, redo_stack);
}