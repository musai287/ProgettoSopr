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
#include "piddini.h"
/*Magica funzione per rendere non
 *bloccante le pipe nella comunicazione tra di loro
 *(bisogna aggiungerla nel main richiamando le pipe)
 */
int punteggio = 0; // Punteggio totale del giocatore
int tempoTotale = 30; // Tempo massimo per ogni manche


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

void finestre() {
    refresh();
    vita = newwin(3, COLS, 0, 0);
    gioco = newwin(LINES-3, COLS, 3, 0);
    wbkgd(vita, COLOR_PAIR(3)); // Imposta il colore di sfondo della finestra vita
    wbkgd(gioco, COLOR_PAIR(2)); // Imposta il colore di sfondo della finestra gioco
    box(vita, 0, 0);    // Bordo della finestra vita
    box(gioco, 0, 0);   // Bordo della finestra gioco
    wrefresh(vita);     // Aggiorna la finestra vita
    wrefresh(gioco);    // Aggiorna la finestra gioco
}

void gestisci_vite(int vite, time_t start_time) {
    werase(vita); // Cancella il contenuto della finestra vita
    box(vita, 0, 0); // Crea il bordo della finestra vita
    int tempoRimanente = 30 - (int)(time(NULL) - start_time);

    /*stampe della finestra superiore*/
    mvwprintw(vita, 1, 1, "Vite: ");
    mvwprintw(vita, 1, COLS / 2, "Tempo: %d", tempoRimanente);
    mvwprintw(vita, 1, COLS -15 , "punti %d", punteggio);
    for (int i = 0; i < vite; i++) {
        mvwprintw(vita, 1, i + 6, "*");
        wrefresh(vita); // Aggiorna la finestra vita
    }
}

void funzionamento_gioco(Frog frog, Crocodile croco[],int numCroco,Entity proiettile[],Entity granata[],int *pipefd, int *pipeEvent) {

    Entity msg;
    Event evento;
    int manche = 1;
    time_t start_time = time(NULL);
    Map tana[5];
    for (int i = 0; i < 5; i++) {
        tana[i] = initTana();
        tana[i].x = 2 + i * 10;
        tana[i].y = 1;
    }
    creaRano(frog, pipefd, pipeEvent);
    creaCroco(croco, numCroco,pipefd);
    //creaGranata(granata, pipefd, pipeEvent, frog);
    //creaProiettile(proiettile, pipefd, pipeEvent, numCroco, croco);
    int proiettileOn = 0;
    while(1){
        if(proiettileOn == 0){
            proiettileOn = 1;
            creaProiettile(proiettile, pipefd, pipeEvent, numCroco, croco);
        }

        int input = movimento();
        if (input == ' ') {
            creaGranata(granata, pipefd, pipeEvent, frog);
            // for (int i=0; i <2; i++){
                // if (granata[i].pid > 0) {  // Se il processo esiste
                
                //     kill(granata[i].pid, SIGKILL); 
                //     waitpid(granata[i].pid, NULL,0); // Aspetta la terminazione                        
                //     granata[i].pid = 0;  // Resetta il PID dopo la terminazione
                // }
            // }
        }
        

	    gestisci_vite(frog.lives, start_time);
        // Leggi i messaggi dalla pipe
        if (read(pipefd[0], &msg, sizeof(Entity)) > 0) {
            if (msg.id == 0) {
                // Aggiorna la posizione della rana
                frog.base.x = msg.x;
                frog.base.y = msg.y;
            }
            else if (msg.id >=1 && msg.id < numCroco+1) {
                // Aggiorna i coccodrilli
                for (int i = 0; i < numCroco; i++) {
                    if (croco[i].base.id == msg.id) {
                        croco[i].base.x = msg.x;
                        croco[i].base.y = msg.y;
                    }
                }
            }
            else if(msg.id >=30 && msg.id < 59) {
                // Aggiorna il proiettile
                for (int i = 0; i < numCroco; i++) {
                    if (proiettile[i].id == msg.id) {
                        proiettile[i].x = msg.x;
                        proiettile[i].y = msg.y;
                    }
                }
                
            }
            else if(msg.id >= 60 && msg.id <= 70){
                for (int i = 0; i < 2; i++) {
                    if (granata[i].id == msg.id) {
                        granata[i].x = msg.x;
                        granata[i].y = msg.y;
                    }
                }
            }
        }
        /*collisioni*/

        int collisionFlag = ranaSuCroco(&frog, croco, numCroco);
        if (collisionFlag) {
            evento.tipo = 2;
            evento.data = collisionFlag;  // Evento che dice alla rana di seguire il coccodrillo
            write(pipeEvent[1], &evento, sizeof(Event));
        } else {
            evento.tipo = 0;
            evento.data = collisionFlag;  // Tipo di evento che dice alla rana di fare un movimento
            write(pipeEvent[1], &evento, sizeof(Event));
        }

        int fiumeFlag = ranaInFiume(&frog, croco, numCroco);
            if(fiumeFlag){
                evento.tipo = 3;
                evento.data = fiumeFlag;
                write(pipeEvent[1], &evento, sizeof(Event)); // Evento che dice alla rana di morire
                frog.lives--;
                start_time = time(NULL);
            }else{
                 evento.tipo = 0;
                 evento.data = fiumeFlag;  // Tipo di evento che dice alla rana di fare un movimento
            }

        int tanaFlag = ranaInTana(&frog, tana);
        if (tanaFlag) {
            evento.tipo = 4;
            evento.data = tanaFlag;  // Tipo di evento che dice alla rana di fare un movimento
            write(pipeEvent[1], &evento, sizeof(Event));
            manche++;
            int tempoImpiegato = (int)(time(NULL) - start_time);
            int tempoRimanente = tempoTotale - tempoImpiegato;
            float moltiplicatore = 1.0 + manche * 0.5 + (float)tempoRimanente / tempoTotale;
            punteggio += (int)(moltiplicatore * 100); // Aggiungi 100 punti base moltiplicati
            start_time = time(NULL);
        }else {
            evento.tipo = 0;
            evento.data = tanaFlag;  // Tipo di evento che dice alla rana di fare un movimento
        }

        int proiettileFlag = ranaProiettile(&frog, proiettile, numCroco);
        if (proiettileFlag) {
            evento.tipo = 6;
            evento.data = proiettileFlag;  // Evento che dice alla rana di morire
            write(pipeEvent[1], &evento, sizeof(Event));
            frog.lives--;

            // int indexColpito = evento.data - 30; // Calcola l'indice del proiettile dall'ID
            //creaProiettile(proiettile, pipefd, pipeEvent, numCroco, croco);

            start_time = time(NULL);


        } else {
            evento.tipo = 0;
            evento.data = proiettileFlag;  // Tipo di evento che dice alla rana di fare un movimento
        }

        int tempoRimanente = 30 - (int)(time(NULL) - start_time);
        if (tempoRimanente <= 0) {
            evento.tipo = 8;
            evento.data = 0;
            write(pipeEvent[1], &evento, sizeof(Event));
            start_time = time(NULL);
            frog.lives--;
        }
        int proiettileFuoriFlag = proiettileFuori(proiettile);
        if (proiettileFuoriFlag) {
            evento.tipo = 10;
            evento.data = proiettileFuoriFlag;  // Evento che dice al proiettile di tornare alla posizione iniziale
            write(pipeEvent[1], &evento, sizeof(Event));
            proiettileOn = 0;
        } else {
            evento.tipo = 0;
            evento.data = proiettileFuoriFlag;
            }

        /*win lose condition*/

        if (manche == 6) {
            mvprintw(LINES / 2, COLS / 2 - 5, "HAI VINTO");
            refresh();
            usleep(DELAYCLOSED);
            endwin();
            exit(0);
        }
        if (frog.lives == 0) {
            mvprintw(LINES / 2, COLS / 2 -5, "HAI PERSO");
					refresh();
					usleep(DELAYCLOSED);
					endwin();
					exit(0);

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

        wattron(gioco, COLOR_PAIR(1));
        stampCocco(gioco, numCroco, croco);
        wattroff(gioco, COLOR_PAIR(1));

        wattron(gioco, COLOR_PAIR(5));
        for (int i = 0; i < 2; i++){
        stampaEntity(gioco, &granata[i]);}

        for (int i = 0; i < numCroco; i++) {
        stampaEntity(gioco, &proiettile[i]);}
		wattroff(gioco, COLOR_PAIR(5));

        mvwprintw(gioco, 1, 1, "Letto messaggio: id=%2d, x=%2d, y=%2d \n",
                                                  msg.id, msg.x, msg.y);

         mvwprintw(gioco, 2, 1, "evento tipo =%d evento data = %d manche = %d",
                                              evento.tipo, evento.data, manche);
        stampaEntity(gioco, &frog.base);
        box(gioco, 0, 0);
        wrefresh(gioco);
        /*richiamo la funzione delle vite.*/
	}
}

