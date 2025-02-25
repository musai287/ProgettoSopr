#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include "croco.h"
#include "struct.h"

void *threadCroco(void *arg) {
    Crocodile *croco = (Crocodile *)arg;

    // Troviamo la SharedData da coccodrillo se ci serve. Esempio:
    // Non l'abbiamo a portata di mano, potremmo passare un struct con {SharedData*, Crocodile*}
    // Oppure, per questa demo, passiamo solo Crocodile* e produciamo i msg se avessimo un reference a sd->buffer.
    // In real code, puoi passare un "CrocoThreadArgs" con .sd e .croco.
    // Qui, per semplicità, "faccio finta" che non usiamo messaggi. OPPURE, ti mostro come potresti fare.

    // *** ESEMPIO => SENZA MESSAGGI: muoviamo direttamente
    while (1) {
        if (croco->direction == 1) {
            croco->base.entity_move(&croco->base, 1, 0);
            if (croco->base.x > COLS + 4) {
                croco->base.x = 1;
            }
        } else {
            croco->base.entity_move(&croco->base, -1, 0);
            if (croco->base.x <= -7) {
                croco->base.x = COLS;
            }
        }
        usleep(200000);
    }

    pthread_exit(NULL);
}

void stampCocco(WINDOW *gioco, Crocodile croco[]) {
    // Stampa tutti i coccodrilli (MAX_CROCO).
    // In funzionamento_gioco userai i primi sd->numCroco.
    for (int i = 0; i < MAX_CROCO; i++) {
        if (croco[i].base.id == 0) continue; // coccodrillo non usato?
        if (croco[i].direction == 1) {
            stampaEntity(gioco, &(croco[i].base));
        } else {
            antiStampaEntity(gioco, &(croco[i].base));
        }
    }
}

void *threadProiettile(void *arg) {
    Entity *proiettile = (Entity *)arg;
    srand(time(NULL) + proiettile->id);

    while (1) {
        proiettile->entity_move(proiettile, 1, 0);
        if (proiettile->x >= COLS) {
            proiettile->x = 0; // Riparte da sinistra
        }
        usleep(100000);
    }

    pthread_exit(NULL);
}
