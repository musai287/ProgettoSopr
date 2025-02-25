#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include "struct.h"
#include "collisioni.h"
#include "funzionamento.h"
#include "frog.h"
#include "croco.h"

int punteggio = 0;
int tempoTotale = 30;

void finestre() {
    refresh();
    vita = newwin(3, COLS, 0, 0);
    gioco = newwin(LINES - 3, COLS, 3, 0);
    wbkgd(vita, COLOR_PAIR(3));
    wbkgd(gioco, COLOR_PAIR(2));
    box(vita, 0, 0);
    box(gioco, 0, 0);
    wrefresh(vita);
    wrefresh(gioco);
}

void gestisci_vite(int vite, time_t start_time) {
    werase(vita);
    box(vita, 0, 0);
    int tempoRimanente = 30 - (int)(time(NULL) - start_time);

    mvwprintw(vita, 1, 1, "Vite: ");
    mvwprintw(vita, 1, COLS / 2, "Tempo: %d", tempoRimanente);
    mvwprintw(vita, 1, COLS - 15, "punti %d", punteggio);

    for (int i = 0; i < vite; i++) {
        mvwprintw(vita, 1, i + 6, "*");
    }
    wrefresh(vita);
}

void funzionamento_gioco(SharedData *sd) {
    srand(time(NULL));
    int proiettileOn = 0;
    time_t start_time = time(NULL);

    // 5 tane
    Map tana[5];
    for (int i = 0; i < 5; i++) {
        tana[i] = initTana();
        tana[i].x = 2 + i*10;
        tana[i].y = 1;
    }

    sd->manche = 1;

    while (!sd->gameOver) {
        // Consumo un messaggio dal buffer
        Messaggio msg = consumeMessaggio(&sd->buffer);

        // Proteggo l'accesso a sd con un mutex se voglio
        pthread_mutex_lock(&sd->lock);

        switch (msg.tipo) {
            case MSG_MOVIMENTO:
                // Se id=1 => frog
                if (msg.id == 1) {
                    sd->frog.base.x += msg.dati.movimento.dx;
                    sd->frog.base.y += msg.dati.movimento.dy;
                } else {
                    // coccodrilli => id = 2..(1+numCroco)
                    for (int i = 0; i < sd->numCroco; i++) {
                        if (msg.id == i + 2) {
                            sd->croco[i].base.x += msg.dati.movimento.dx;
                            sd->croco[i].base.y += msg.dati.movimento.dy;
                            break;
                        }
                    }
                }
                break;

            case MSG_POSIZIONE:
                // Se id=1 => frog
                if (msg.id == 1) {
                    sd->frog.base.x = msg.dati.posizione.x;
                    sd->frog.base.y = msg.dati.posizione.y;
                } else {
                    for (int i = 0; i < sd->numCroco; i++) {
                        if (msg.id == i+2) {
                            sd->croco[i].base.x = msg.dati.posizione.x;
                            sd->croco[i].base.y = msg.dati.posizione.y;
                        }
                    }
                }
                break;

            case MSG_EVENTO:
                // Esempio: collisione
                if (msg.id == 1) {
                    // frog ha generato un evento
                    if (msg.dati.evento.info == 1) {
                        // collisione con coccodrillo?
                        sd->frog.lives--;
                        sd->frog.base.x = (COLS/2)-3;
                        sd->frog.base.y = LINES-5;
                        start_time = time(NULL);
                    }
                }
                break;
        }
        pthread_mutex_unlock(&sd->lock);

        // Gestione tempo
        int tempoRimanente = tempoTotale - (int)(time(NULL) - start_time);
        if (tempoRimanente <= 0) {
            sd->frog.lives--;
            sd->frog.base.x = (COLS/2) - 3;
            sd->frog.base.y = LINES - 5;
            start_time = time(NULL);
        }

        // collisioni: per esempio, se la rana è nel fiume e non su un croco => muore
        if (ranaInFiume(&sd->frog, sd->croco, sd->numCroco)) {
            sd->frog.lives--;
            sd->frog.base.x = (COLS/2)-3;
            sd->frog.base.y = LINES-5;
            start_time = time(NULL);
        }

        if (ranaInTana(&sd->frog, tana)) {
            sd->manche++;
            int tempoImpiegato = (int)(time(NULL) - start_time);
            int tempoRimanente = tempoTotale - tempoImpiegato;
            float moltiplicatore = 1.0 + sd->manche * 0.5 + (float)tempoRimanente / tempoTotale;
            sd->punteggio += (int)(moltiplicatore * 100);
            sd->frog.base.x = (COLS/2)-3;
            sd->frog.base.y = LINES-5;
            start_time = time(NULL);
        }

        // Se la rana è colpita dal proiettile
        int colpita = ranaProiettile(&sd->frog, &sd->proiettile);
        if (colpita) {
            sd->frog.lives--;
            sd->frog.base.x = (COLS/2)-3;
            sd->frog.base.y = LINES-5;
            start_time = time(NULL);
        }

        // vittoria / sconfitta
        if (sd->manche == 6) {
            mvprintw(LINES/2, COLS/2 - 5, "HAI VINTO");
            refresh();
            usleep(DELAYCLOSED);
            endwin();
            sd->gameOver = 1;
            break;
        }
        if (sd->frog.lives <= 0) {
            mvprintw(LINES/2, COLS/2 - 5, "HAI PERSO");
            refresh();
            usleep(DELAYCLOSED);
            endwin();
            sd->gameOver = 1;
            break;
        }

        // Disegno
        gestisci_vite(sd->frog.lives, start_time);

        werase(gioco);
        box(gioco, 0, 0);

        for (int i = 0; i < 5; i++) {
            stampaMap(gioco, &tana[i]);
        }
        // fiume
        wattron(gioco, COLOR_PAIR(4));
        for(int i=0; i<2; i++){
            for(int j=0; j <COLS-2; j++){
                mvwprintw(gioco, 6+i,1+j, "/");
            }
        }
        for(int i=0; i<3; i++){
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
    } // while

    endwin();
}
