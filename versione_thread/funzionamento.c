#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h> // se serve
#include "struct.h"
#include "collisioni.h"   // tu hai già le funzioni di collisione
#include "funzionamento.h"
#include "frog.h"
#include "croco.h"
 

// Contatore di tempo o punteggio globale, se vuoi
int punteggio = 0;
int tempoTotale = 30;

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

void funzionamento_gioco(SharedData *sd, BufferCircolare *buffer) {
    srand(time(NULL));
    int proiettileOn = 0;      
    time_t start_time = time(NULL);

    Map tana[5];
    for (int i = 0; i < 5; i++) {
        tana[i] = initTana();
        tana[i].x = 2 + i * 10;
        tana[i].y = 1;
    }

    sd->manche = 1;

    while (!sd->gameOver) {
        // =========== LEGGI MESSAGGI DAL BUFFER =============
        Messaggio msg = consumeMessaggio(buffer);
        pthread_mutex_lock(&sd->lock);

        switch (msg.tipo) {
            case MSG_MOVIMENTO:
                if (msg.id == 1) { // Rana
                    sd->frog.base.x += msg.dati.movimento.dx;
                    sd->frog.base.y += msg.dati.movimento.dy;
                } else { // Altra entità
                    for (int i = 0; i < sd->numCroco; i++) {
                        if (msg.id == i + 2) {
                            sd->croco[i].x += msg.dati.movimento.dx;
                            sd->croco[i].y += msg.dati.movimento.dy;
                        }
                    }
                }
                break;

            case MSG_POSIZIONE:
                if (msg.id == 1) {
                    sd->frog.base.x = msg.dati.posizione.x;
                    sd->frog.base.y = msg.dati.posizione.y;
                }
                break;

            case MSG_EVENTO:
                if (msg.dati.evento.info == 1) { // Collisione rana-coccodrillo
                    sd->frog.lives--;
                    sd->frog.base.x = (COLS /2) - 3;
                    sd->frog.base.y = LINES - 5;
                    start_time = time(NULL);
                }
                break;
        }

        pthread_mutex_unlock(&sd->lock);

        // =========== GESTIONE TEMPO ============
        int tempoRimanente = tempoTotale - (int)(time(NULL) - start_time);
        if (tempoRimanente <= 0) {
            sd->frog.lives--;
            sd->frog.base.x = (COLS /2) - 3;
            sd->frog.base.y = LINES - 5;
            start_time = time(NULL);
        }

        // =========== CONTROLLO VITTORIA/SCONFITTA ============
        if (sd->manche == 6) {
            mvprintw(LINES / 2, COLS / 2 - 5, "HAI VINTO");
            refresh();
            usleep(DELAYCLOSED);
            endwin();
            sd->gameOver = 1;
            return;
        }
        if (sd->frog.lives <= 0) {
            mvprintw(LINES / 2, COLS / 2 - 5, "HAI PERSO");
            refresh();
            usleep(DELAYCLOSED);
            endwin();
            sd->gameOver = 1;
            return;
        }

        // =========== DISEGNO A SCHERMO ============
        werase(gioco);  
        box(gioco, 0, 0);

        for (int i = 0; i < 5; i++) {
            stampaMap(gioco, &tana[i]);
        }

        wattron(gioco, COLOR_PAIR(4));
        for(int i=0; i<2; i++){
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
        stampCocco(gioco, sd->croco);
        wattroff(gioco, COLOR_PAIR(1));

        wattron(gioco, COLOR_PAIR(5));
        for (int i = 0; i < 2; i++) {
            stampaEntity(gioco, &sd->granata[i]);
        }
        stampaEntity(gioco, &sd->proiettile);
        wattroff(gioco, COLOR_PAIR(5));

        stampaEntity(gioco, &sd->frog.base);

        box(gioco, 0, 0);
        wrefresh(gioco);

        usleep(50000);
    }

    endwin();
}
