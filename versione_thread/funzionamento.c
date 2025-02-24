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

void funzionamento_gioco(SharedData *sd) {
    srand(time(NULL));

    // Inizializziamo alcune variabili di supporto
    int proiettileOn = 0;      // Se il proiettile è “attivo” o meno
    time_t start_time = time(NULL);

    // Inizializziamo 5 tane, come facevi tu
    Map tana[5];
    for (int i = 0; i < 5; i++) {
        tana[i] = initTana();
        tana[i].x = 2 + i * 10;
        tana[i].y = 1;
    }

    // Imposta la manche iniziale
    sd->manche = 1;

    while (!sd->gameOver) {
        // =========== GESTIONE VITE E TEMPO =============
        // Stampa delle vite e del tempo rimanente
        gestisci_vite(sd->frog.lives, start_time);

        // =========== CONTROLLI SU PROIETTILE ============
        // Nel tuo codice originale, creavi un processo per il proiettile quando proiettileOn era 0
        // Ora, invece, il proiettile è gestito da un thread (se vuoi), oppure puoi muoverlo qui.
        // Se usi un thread separato (threadProiettile), puoi lasciare qui solo un controllo logico.
        if (!proiettileOn) {
            // Attiva il proiettile
            // Se hai un threadProiettile, potresti fare la logica di “reset” qui,
            //   es: sd->proiettile.x = ...  e così via
            proiettileOn = 1;
        }

        // =========== LETTURA POSIZIONI / COLLISIONI ============
        // Ora non leggiamo più pipe, ma usiamo i campi di sd->frog, sd->croco[], sd->proiettile, ecc.

        // 1) Collisione rana-coccodrillo
        int collisionFlag = ranaSuCroco(&sd->frog, sd->croco, sd->numCroco);
        if (collisionFlag) {
            // L’originale inviava un evento. Ora puoi gestirlo direttamente.
            // Esempio: la rana si sposta dove si sposta il coccodrillo
            // Oppure riduci vite, come preferivi
            // Nel tuo codice, facevi: evento.tipo = 2 => “la rana è sopra il coccodrillo”
            // Ora:
            // sd->frog.base.x = ...
        } else {
            // Nessuna collisione con coccodrillo
        }

        // 2) Collisione rana-fiume
        int fiumeFlag = ranaInFiume(&sd->frog, sd->croco, sd->numCroco);
        if (fiumeFlag) {
            // Nel codice originale, la rana moriva (vite--) e tornava a inizio
            sd->frog.lives--;
            sd->frog.base.x = (COLS /2) - 3;
            sd->frog.base.y = LINES - 5;
            start_time = time(NULL);  // Resetta il timer per la nuova vita
        }

        // 3) Collisione rana-tana
        int tanaFlag = ranaInTana(&sd->frog, tana);
        if (tanaFlag) {
            // Aumenta manche e calcola punteggio
            sd->manche++;
            int tempoImpiegato = (int)(time(NULL) - start_time);
            int tempoRimanente = tempoTotale - tempoImpiegato;
            float moltiplicatore = 1.0 + sd->manche * 0.5 + (float)tempoRimanente / tempoTotale;
            sd->punteggio += (int)(moltiplicatore * 100);
            // Reset rana
            sd->frog.base.x = (COLS /2) - 3;
            sd->frog.base.y = LINES - 5;
            start_time = time(NULL);
        }

        // 4) Collisione rana-proiettile
        int proiettileFlag = ranaProiettile(&sd->frog, &sd->proiettile);
        if (proiettileFlag) {
            // La rana muore
            sd->frog.lives--;
            // Resetta posizione
            sd->frog.base.x = (COLS /2) - 3;
            sd->frog.base.y = LINES - 5;
            // Resetta timer
            start_time = time(NULL);
        }

        // =========== GESTIONE TEMPO ============
        int tempoRimanente = tempoTotale - (int)(time(NULL) - start_time);
        if (tempoRimanente <= 0) {
            // Tempo scaduto => la rana perde una vita
            sd->frog.lives--;
            // Reset
            sd->frog.base.x = (COLS /2) - 3;
            sd->frog.base.y = LINES - 5;
            start_time = time(NULL);
        }

        // =========== CONTROLLO VITTORIA/SCONFITTA ============
        if (sd->manche == 6) {
            // HAI VINTO
            mvprintw(LINES / 2, COLS / 2 - 5, "HAI VINTO");
            refresh();
            usleep(DELAYCLOSED);
            endwin();
            // Imposta un flag di terminazione, se vuoi
            sd->gameOver = 1;
            return;  // o break
        }
        if (sd->frog.lives <= 0) {
            // HAI PERSO
            mvprintw(LINES / 2, COLS / 2 - 5, "HAI PERSO");
            refresh();
            usleep(DELAYCLOSED);
            endwin();
            sd->gameOver = 1;
            return;  // o break
        }

        // =========== DISEGNO A SCHERMO ============
        werase(gioco);  
        box(gioco, 0, 0);

        // Stampa delle 5 tane
        for (int i = 0; i < 5; i++) {
            stampaMap(gioco, &tana[i]);
        }

        // Fiume colorato
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

        // Stampa coccodrilli
        wattron(gioco, COLOR_PAIR(1));
        stampCocco(gioco, sd->numCroco, sd->croco);
        wattroff(gioco, COLOR_PAIR(1));

        // Stampa granate
        wattron(gioco, COLOR_PAIR(5));
        for (int i = 0; i < 2; i++) {
            stampaEntity(gioco, &sd->granata[i]);
        }
        // Stampa proiettile
        stampaEntity(gioco, &sd->proiettile);
        wattroff(gioco, COLOR_PAIR(5));

        // Stampa la rana
        stampaEntity(gioco, &sd->frog.base);

        box(gioco, 0, 0);
        wrefresh(gioco);

        // Piccola pausa prima del prossimo ciclo
        usleep(50000);
    }

    // Se usciamo dal while => gameOver = 1
    // Eventualmente stampi un messaggio finale
    endwin();
}
