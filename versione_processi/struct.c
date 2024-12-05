#include <ncurses.h>
#include "struct.h"

WINDOW *vita, *gioco;
Fin fin1;
Fin fin2;

void init_fin() {
    // Inizializza le variabili globali dopo aver chiamato initscr()
    fin1 = (Fin){3, COLS, 0, 0};
    fin2 = (Fin){LINES - 3, COLS, 3, 0};
}