#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include "croco.h"
#include "struct.h"

void *threadCroco(void *arg) {
    Crocodile *croco = (Crocodile *)arg;
    SharedData *sd = croco->base.sd;
    int velocita = croco->base.y *10000;
    while (!sd->gameOver) {
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
        Messaggio msg;
        msg.tipo = MSG_POSIZIONE;
        msg.id   = croco->base.id; // es. i+2
        msg.dati.posizione.x = croco->base.x;
        msg.dati.posizione.y = croco->base.y;

        produceMessaggio(&sd->buffer, msg);

        usleep(200000+velocita);
    }

    pthread_exit(NULL);
}

void stampCocco(WINDOW *gioco, Crocodile croco[]) {
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
    SharedData *sd = proiettile->sd;
    srand(time(NULL) + proiettile->id);

    int indexCroco = rand() % sd->numCroco;
   
    while (!sd->gameOver) {
        if (sd->croco[indexCroco].direction == 1) {
            proiettile->entity_move(proiettile, 1, 0);
        } else {
            proiettile->entity_move(proiettile, -1, 0);
        }
        if (proiettile->x >= COLS-1 || proiettile->x <= -6) {
            indexCroco = rand() % sd->numCroco;
            proiettile->x = sd->croco[indexCroco].base.x;
            proiettile->y = sd->croco[indexCroco].base.y;
        }
    
        Messaggio msg;
        msg.tipo = MSG_POSIZIONE;
        msg.id   = proiettile->id; // 30
        msg.dati.posizione.x = proiettile->x;
        msg.dati.posizione.y = proiettile->y;
        produceMessaggio(&sd->buffer, msg);
        usleep(100000);
    }

    pthread_exit(NULL);
}
