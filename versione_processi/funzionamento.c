#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>

#include "struct.h"
#include "funzionamento.h"
#include "frog.h"
#include "croco.h"


void finestre(Fin *fin1, Fin *fin2) {
    refresh();
    vita = newwin(fin1->height, fin1->width, fin1->starty, fin1->startx);
    gioco = newwin(fin2->height, fin2->width, fin2->starty, fin2->startx);
    wbkgd(vita, COLOR_PAIR(3)); // Imposta il colore di sfondo della finestra vita
    wbkgd(gioco, COLOR_PAIR(4)); // Imposta il colore di sfondo della finestra gioco
    box(vita, 0, 0);    // Bordo della finestra vita
    box(gioco, 0, 0); // Bordo della finestra gioco
    wrefresh(vita);  // Aggiorna la finestra vita
    wrefresh(gioco); // Aggiorna la finestra gioco
}

void gestisci_vite(int vite, time_t start_time) {
    werase(vita); // Cancella il contenuto della finestra vita
    box(vita, 0, 0); // Crea il bordo della finestra vita
    mvwprintw(vita, 1, 1, "Vite: ");
    mvwprintw(vita, 1, COLS / 2, "Tempo: %d", (int)(time(NULL) - start_time));
    for (int i = 0; i < vite; i++) {
        mvwprintw(vita, 1, i + 6, "*");
        wrefresh(vita); // Aggiorna la finestra vita
    }
}
void funzionamento_gioco (int pipe_fd){
	MesPos message;
	initSRana();
	initSCroco();
	int vite = 3;
    time_t start_time = time(NULL);
	//time_t last_trap_update_time = time(NULL);
	while(1){
		if (read(pipe_fd, &message, sizeof(MesPos)) > 0) {
			if (message.tipo == 0) {
                rana = message;
			}
            else if (message.tipo == 1){
                croco = message;
            }
			werase(gioco);  // Cancella il contenuto della finestra gioco
            box(gioco, 0, 0); // Crea il bordo della finestra gioco
			//questo controllo if  else if serve per capire chi sta usando il buffer
			werase(gioco);
            box(gioco,0,0);
      mvwprintw(gioco,croco.x,   croco.y, "_______");
			mvwprintw(gioco,croco.x+1, croco.y, "|     |");
			mvwprintw(gioco,croco.x+2, croco.y, "|     |");
			mvwprintw(gioco,croco.x+3, croco.y, "|_____|"); //simbolo usato per rappresentare il personaggio
			attron(A_REVERSE);
			mvwprintw(gioco,rana.x-1, rana.y, "___");
			mvwprintw(gioco,rana.x, rana.y,   "|_|"); //simbolo usato per rappresentare il personaggio
		    attron(A_REVERSE);
			wrefresh(gioco);
			gestisci_vite(vite, start_time); 

			if (rana.x == croco.x && rana.y == croco.y){
				mvprintw(LINES / 2, COLS / 2 -5, "preso");
				refresh();
				usleep(DELAYCLOSED);
				endwin();
				exit(0);
			} //win condition
		}
	}
}
