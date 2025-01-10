#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>
#include <string.h>
#include <fcntl.h>

#include "struct.h"
#include "funzionamento.h"
#include "frog.h"
#include "croco.h"
/*Magica funzione per rendere non
 *bloccante le pipe nella comunicazione tra di loro 
 *(bisogna aggiungerla nel main richiamando le pipe)
 */
void setNonBlocking(int pipe_fd) {
    int flags = fcntl(pipe_fd, F_GETFL, 0);
    if (flags == -1) {
        perror("fcntl F_GETFL");
        exit(EXIT_FAILURE);
    }

    if (fcntl(pipe_fd, F_SETFL, flags | O_NONBLOCK) == -1) {
        perror("fcntl F_SETFL");
        exit(EXIT_FAILURE);
    }
}

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
void funzionamento_gioco (int numCroco,int pipeRana, int pipeCroco,int positions[]) {
	Message msg;
	initSRana();
	initSCroco();
	int vite = 3;
	
    time_t start_time = time(NULL);
	//time_t last_trap_update_time = time(NULL);
	while(1){
		if (read(pipeRana, &rana, sizeof(MesPos)) > 0) {
			if (read(pipeCroco, &msg, sizeof(MesPos)) > 0) {
                    if (msg.event == 1) {
                // Evento: uscito dallo schermo (gestione facoltativa)
            			} 
					else {
                // Aggiorna posizione
                		positions[msg.id] = msg.event;
            		}
				//questo controllo if  else if serve per capire chi sta usando il buffer

				werase(gioco);  // Cancella il contenuto della finestra gioco
            	box(gioco, 0, 0); // Crea il bordo della finestra gioco
				//werase(gioco);
            	//box(gioco,0,0);
				stampCocco(numCroco,pipeCroco,positions);
				stampRana(pipeRana);
				/*
				for (int i = 0; i < numCroco; i++) {
					int row = i / 3 + 1;
					int col;
					if (row % 2 == 1) {
						col = (i % 3) * 10;
					} else {
						col = COLS - (i % 3) * 10 - 10;
					}
					mvwprintw(gioco, row, col + positions[i], " [====]=");
				}
				attron(A_REVERSE);
				*/
			/*
				mvwprintw(gioco,rana.x-1, rana.y, "___");
				mvwprintw(gioco,rana.x, rana.y,   "|_|"); //simbolo usato per rappresentare il personaggio
		    	attron(A_REVERSE);
			*/
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
}
