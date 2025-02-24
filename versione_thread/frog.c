#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include "frog.h"
#include "struct.h"


// Funzione di input non bloccante
int movimento() {
    int input = getch(); // getch non-bloccante se nodelay(stdscr, TRUE)
    return input;
}

/*
 * Thread che sostituisce "processoRana()".
 * Arg = (SharedData *) contenente frog, granate, ecc.
 */
void *threadRana(void *arg) {
    SharedData *sd = (SharedData *)arg;
    Frog *frog = &sd->frog;

    while (!sd->gameOver) {
        // Legge input da tastiera
        int input = movimento();
        int new_x = frog->base.x;
        int new_y = frog->base.y;

        switch (input) {
            case KEY_UP:    new_y -= 1; break;
            case KEY_DOWN:  new_y += 1; break;
            case KEY_LEFT:  new_x -= 1; break;
            case KEY_RIGHT: new_x += 1; break;

            // Sparo granate a destra/sinistra se si preme spazio
            case ' ':
                // Creiamo due "threadGranata"? Oppure riusiamo un array e le spostiamo?
                // Qui come esempio, lanciamo i due thread granata.
                // (In realtà potresti semplicemente settare le coordinate delle due granate e farle muovere in un threadGranata)
                for (int i = 0; i < 2; i++) {
                    sd->granata[i].x = frog->base.x + (i == 0 ? -1 : +3);
                    sd->granata[i].y = frog->base.y;
                }
                break;
        }

        // Verifica i limiti dello schermo
        if (new_x >= 1 && new_x + frog->base.sprite.larghezza <= COLS &&
            new_y >= 0 && new_y + frog->base.sprite.lunghezza <= LINES - 4)
        {
            frog->base.entity_move(&frog->base, new_x - frog->base.x, new_y - frog->base.y);
        }

        // Eventuali controlli "evento" che prima leggevano da pipeEvent
        // Ora potresti avere una condizione globale, o potresti controllare collisioni qui
        // ... (omesso)

        // Esempio: se la rana "muore", la rimettiamo alla base
        // frog->base.x = (COLS/2) - 3;
        // frog->base.y = LINES - 5;

        // Sleep (per non saturare la CPU)
        usleep(50000);
    }

    pthread_exit(NULL);
}

/*
 * Thread che sostituisce "processoGranata()".
 * Qui muoviamo le granate (destra/sinistra) finché sono in gioco.
 * In pratica: loop, sposta, collisioni, etc.
 */
void *threadGranata(void *arg) {
    // Esempio: arg è un indice che ci dice quale granata muoviamo
    // Oppure passiamo direttamente (Entity *)?
    // Qui dimostro se passassimo un (Entity *):

    Entity *granata = (Entity *)arg;

    while (1) {
        // Se la granata è "disattivata", potremmo fare break
        // Oppure se gameOver => break

        if (granata->id == 61) {
            entity_move(granata, 1, 0);
            if (granata->x > COLS - 1) {
                // Fine corsa, posso eventualmente rimetterla fuori gioco
                granata->x = -100; // la nascondo
            }
        } else {
            // id = 60
            entity_move(granata, -1, 0);
            if (granata->x < 0) {
                granata->x = -100; 
            }
        }
        usleep(75000);
    }

    pthread_exit(NULL);
}
