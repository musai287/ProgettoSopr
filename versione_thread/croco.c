#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include "croco.h"
#include "struct.h"


void *threadCroco(void *arg) {
    // arg è un puntatore a un singolo Crocodile
    Crocodile *croco = (Crocodile *) arg;
    
    // (Opzionale) Inizializza il seme per i numeri casuali
    // Se ogni thread ne ha bisogno, ad esempio:
    srand(time(NULL) + croco->base.id);

    // Ciclo infinito per aggiornare il movimento del coccodrillo
    while (1) {
        if (croco->direction == 1) {
            croco->base.entity_move(&croco->base, 1, 0);
            if (croco->base.x > COLS + 4) {
                croco->base.x = 1;
            }
        } else { // supponiamo che direction == 2
            croco->base.entity_move(&croco->base, -1, 0);
            if (croco->base.x <= -7) {
                croco->base.x = COLS;
            }
        }
        usleep(200000); // Regolazione della velocità
    }
    
    // Non raggiunto, ma per completezza:
    pthread_exit(NULL);
    return NULL;
}



void stampCocco(WINDOW *gioco, Crocodile croco[]) {
    for (int i = 0; i < MAX_CROCO; i++) {
        if (croco[i].direction == 1) {
            stampaEntity(gioco, &(croco[i].base));
        } else {
            antiStampaEntity(gioco, &(croco[i].base));
        }
    }
}

void *threadProiettile(void *arg) {
    // Analogo discorso, passiamo un (Entity *) o (SharedData *)?
    // Se passiamo un (Entity *), muoviamo il proiettile in un loop
    // Se passiamo (SharedData *), allora cerchiamo croco casuali e così via

    Entity *proiettile = (Entity *)arg;
    srand(time(NULL) + proiettile->id);

    while (1) {
        // Esempio banale: proiettile si muove a dx
        entity_move(proiettile, 1, 0);

        if (proiettile->x >= COLS) {
            proiettile->x = 0; // riparte da sinistra
        }
        usleep(100000);
    }

    pthread_exit(NULL);
}
