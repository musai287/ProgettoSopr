#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include "frog.h"
#include "struct.h"

int movimento() {
    // getch non bloccante se nodelay(stdscr, TRUE)
    return getch();
}

void *threadRana(void *arg) {
    SharedData *sd = (SharedData *)arg;
    Frog *frog = &sd->frog;

    while (!sd->gameOver) {
        int input = movimento();
        if (input == ERR) {
            // nessun tasto premuto
            usleep(30000);
            continue;
        }

        int new_x = frog->base.x;
        int new_y = frog->base.y;

        switch (input) {
            case KEY_UP:    new_y -= 1; break;
            case KEY_DOWN:  new_y += 1; break;
            case KEY_LEFT:  new_x -= 1; break;
            case KEY_RIGHT: new_x += 1; break;
            case ' ':
                // Spara granate (aggiorna posizioni)
                for (int i = 0; i < 2; i++) {
                    sd->granata[i].x = frog->base.x + (i == 0 ? -1 : 3);
                    sd->granata[i].y = frog->base.y;
                }
                break;
        }

        // Calcoliamo dx, dy
        int dx = new_x - frog->base.x;
        int dy = new_y - frog->base.y;

        // Verifica i limiti
        if (new_x >= 1 &&
            new_x + frog->base.sprite.larghezza <= COLS &&
            new_y >= 0 &&
            new_y + frog->base.sprite.lunghezza <= LINES - 4) 
        {
            // Invece di muovere direttamente la rana, inviamo un Messaggio al buffer
            Messaggio msg;
            msg.tipo = MSG_MOVIMENTO;
            msg.id = 1;  // la rana ha id=1
            msg.dati.movimento.dx = dx;
            msg.dati.movimento.dy = dy;

            produceMessaggio(&sd->buffer, msg);
        }

        usleep(50000);
    }

    pthread_exit(NULL);
}

void *threadGranata(void *arg) {
    Entity *granata = (Entity *)arg;

    while (1) {
        // Se la granata ha id=60 => si muove a sinistra
        if (granata->id == 60) {
            granata->entity_move(granata, -1, 0);
            if (granata->x < 0) {
                granata->x = -100; // la nascondo
            }
        } else {
            // id=61 => a destra
            granata->entity_move(granata, +1, 0);
            if (granata->x >= COLS) {
                granata->x = COLS + 100; 
            }
        }
        usleep(75000);
    }

    pthread_exit(NULL);
}
