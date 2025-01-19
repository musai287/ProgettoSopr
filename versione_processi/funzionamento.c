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
#include "collisioni.h"
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
    wbkgd(gioco, COLOR_PAIR(2)); // Imposta il colore di sfondo della finestra gioco
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
void funzionamento_gioco(Frog frog, Crocodile croco[],int numCroco,int pipefd, int pipeEvent) {
	Entity msg;
    Event evento;
    Map tana[5];
    for (int i = 0; i < 5; i++) {
        tana[i] = initTana();
        tana[i].x = 12 + i * 10;
        tana[i].y = 1;    
    }
    

    time_t start_time = time(NULL);

	while(1){
        // Leggi i messaggi dalla pipe
        if (read(pipefd, &msg, sizeof(Entity)) > 0) {
            if (msg.id == 0) {
                // Aggiorna la posizione della rana
                frog.base.x = msg.x;
                frog.base.y = msg.y;
            } else {
                // Aggiorna i coccodrilli
                for (int i = 0; i < numCroco; i++) {
                    if (croco[i].base.id == msg.id) {
                        croco[i].base.x = msg.x;
                        croco[i].base.y = msg.y;
                    }
                } 
            }
        }
         int collisionFlag = ranaSuCroco(&frog, croco, numCroco);

        if (collisionFlag) {
            
            evento.tipo = 2;
            evento.data = collisionFlag;  // Evento che dice alla rana di seguire il coccodrillo
            write(pipeEvent, &evento, sizeof(Event));

        } else {
            evento.tipo = 0;
            evento.data = collisionFlag;  // Tipo di evento che dice alla rana di fare un movimento
            write(pipeEvent, &evento, sizeof(Event));

        }
        int fiumeFlag = ranaInFiume(&frog, croco, numCroco);
        // if(fiumeFlag){
            // evento.tipo = 3;
            // evento.data = fiumeFlag; 
            // write(pipeEvent, &evento, sizeof(Event)); // Evento che dice alla rana di morire    
            // frog.lives--;
            // evento.tipo = 0;
        // }else {
            // evento.tipo = 0;
            // evento.data = collisionFlag;  // Tipo di evento che dice alla rana di fare un movimento
            // write(pipeEvent, &evento, sizeof(Event));
        // }
        if (frog.lives == 0) {
            mvprintw(LINES / 2, COLS / 2 -5, "HAI PERSO");
					refresh();
					usleep(DELAYCLOSED);
					endwin();
					exit(0);
				 //win condition
        }

        /* semplicemente le stampe. */
        werase(gioco);  // Cancella il contenuto della finestra gioco
        box(gioco, 0, 0); // Crea il bordo della finestra gioco
        
        for (int i = 0; i < 5; i++) {
            stampaMap(gioco, &tana[i]);
        }
        wattron(gioco, COLOR_PAIR(4));
        for(int i=0; i<2;i++){
            for(int j=0; j <COLS-2; j++){
                mvwprintw(gioco, 6+i,1+j, "/");
            }
        }
        for(int i=0; i<3;i++){
            for(int j=0; j <COLS-2; j++){
                mvwprintw(gioco, 16+i,1+j, "/");
            }
        }
        wattroff(gioco, COLOR_PAIR(4));
        mvwprintw(gioco, 18, 1, "Letto messaggio: id=%2d, x=%2d, y=%2d \n",
                                                  msg.id, msg.x, msg.y);
        mvwprintw(gioco, 17, 1, "evento tipo =%d evento data = %d",
                                        evento.tipo, evento.data);

        wattron(gioco, COLOR_PAIR(1));
        stampCocco(gioco, numCroco, croco);
        wattroff(gioco, COLOR_PAIR(1));

        wattron(gioco, COLOR_PAIR(5));
        stampaEntity(gioco, &frog.base);
		wattroff(gioco, COLOR_PAIR(5));
        
        wrefresh(gioco);
		gestisci_vite(frog.lives, start_time);
	}
}

//6 7  17 18    8 15