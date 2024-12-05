#include <ncurses.h>
#include "struct.h"

WINDOW *vita, *gioco;
Fin fin1;
Fin fin2;
MesPos croco;
MesPos rana;

void initFin() {
    // Inizializza le variabili globali dopo aver chiamato initscr()
    fin1 = (Fin){3, COLS, 0, 0};
    fin2 = (Fin){LINES - 3, COLS, 3, 0};
}
void initSCroco(){
    // Inizializza le variabili globali per croco   
    croco = (MesPos){1, 1, 1};
}
void initSRana(){
    // Inizializza le variabili globali per rana
    rana = (MesPos){0, LINES - 6, COLS - 2};
}