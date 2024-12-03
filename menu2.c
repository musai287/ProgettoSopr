#include <ncurses.h>
#include <stdlib.h>
typedef struct Fin {
    int height;
    int width;
    int starty;
    int startx;
} Fin;

int main() {
    initscr(); // Inizializza ncurses
    noecho();  // Disabilita l'eco dei tasti premuti

    start_color(); // Abilita i colori
    init_pair(1, COLOR_WHITE, COLOR_BLUE);
    init_pair(2, COLOR_YELLOW, COLOR_RED);

    curs_set(FALSE); // Nasconde il cursore
    bkgd(COLOR_PAIR(1)); // Imposta il colore dello sfondo
    keypad(stdscr, TRUE); // Abilita i tasti speciali per stdscr

    box(stdscr, 0, 0); // Disegna un bordo attorno alla finestra principale
    mvprintw(2, 5, "Press any key to show another window");
    wrefresh(stdscr); // Aggiorna la finestra principale

    //wgetch(stdscr); // Aspetta un input dell'utente

    // Creazione di una nuova finestra
    Fin ciao = {10, 30, 5, 10};
    WINDOW *finestra = newwin(ciao.height, ciao.width, ciao.starty, ciao.startx);
    wbkgd(finestra, COLOR_PAIR(2)); // Imposta il colore della nuova finestra
    box(finestra, 0, 0); // Disegna un bordo attorno alla nuova finestra
    mvwprintw(finestra, 1, 1, "Hello from new window!");
    mvwprintw(finestra, 2, 1, "Press any key to exit");
    wrefresh(finestra); // Aggiorna la nuova finestra

    wgetch(finestra); // Aspetta un input dell'utente nella nuova finestra

    delwin(finestra); // Elimina la nuova finestra
    endwin(); // Ripristina lo stato del terminale
    exit(0);
    return 0;
}
