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
void setNonBlocking(int pipefd) {
    int flags = fcntl(pipefd, F_GETFL, 0);
    if (flags == -1) {
        perror("fcntl F_GETFL");
        exit(EXIT_FAILURE);
    }

    if (fcntl(pipefd, F_SETFL, flags | O_NONBLOCK) == -1) {
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
void funzionamento_gioco(Frog frog, Crocodile croco[],int numCroco,int pipefd) {
	Entity msg;
	
    time_t start_time = time(NULL);
	//time_t last_trap_update_time = time(NULL);
   // aggiornaPosizioni(numCroco, croco);
	while(1){
		while (1) {
        // Leggi i messaggi dalla pipe
        while (read(pipefd, &msg, sizeof(Entity)) > 0) {
            if (msg.id == 0) {
                // Messaggio dalla rana
                frog.base.x = msg.x;
                frog.base.y = msg.y;
            } else {
                for (int i = 0; i < numCroco; i++) {
                    if (croco[i].base.id == msg.id) { // Trova il coccodrillo corretto
                        croco[i].base.x = msg.x;
                        croco[i].base.y = msg.y;
                    
                }
            }
        }
		//questo controllo if  else if serve per capire chi sta usando il buffer
		werase(gioco);  // Cancella il contenuto della finestra gioco
        box(gioco, 0, 0); // Crea il bordo della finestra gioco
        mvwprintw(gioco, 15, 15, "Letto messaggio: id=%d, x=%d, y=%d\n",
                                                  msg.id, msg.x, msg.y);
		
        stampCocco(gioco, numCroco, croco);
        
        
        stampaEntity(gioco, &frog.base);
		wrefresh(gioco);
		gestisci_vite(frog.lives, start_time);	
			}
		}
	}
}
