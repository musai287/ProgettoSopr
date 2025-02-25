#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <stdio.h>
#include "piddini.h"

// Le funzioni thread che eseguono il “processo” di ciascuna entità
// (da implementare/adattare secondo la logica del gioco)
void *frog_thread(void *arg);
void *croco_thread(void *arg);
void *proiettile_thread(void *arg);
void *granata_thread(void *arg);

/*--- Strutture per passare gli argomenti ai thread ---*/
typedef struct {
    Frog *frog;
    int pipefd[2];
    int pipeEvent[2];
    Entity *granata;      // Puntatore al primo elemento dell'array granata
    Entity *proiettile;
} FrogThreadArgs;

typedef struct {
    Crocodile *croco;
    int pipefd[2];
} CrocoThreadArgs;

typedef struct {
    Entity *proiettile;
    int pipefd[2];
    int pipeEvent[2];
    int numCroco;
    Crocodile *croco;  // Coccodrillo scelto (per impostare posizione, ecc.)
    Entity *granata;
} ProiettileThreadArgs;

typedef struct {
    Entity *granata;
    int pipefd;  // In questo caso, il pipefd è passato come singolo valore
    Entity *proiettile;
} GranataThreadArgs;

/*--- Implementazione delle funzioni ---*/

void creaPipe(int pipe_fd[2]) {
    if (pipe(pipe_fd) == -1) {
        perror("Errore nella creazione della pipe");
        exit(1);
    }
}

void creaRano(Frog *frog, int pipefd[2], int pipeEvent[2], Entity granata[], Entity *proiettile) {
    // Alloca e prepara gli argomenti per il thread della rana
    FrogThreadArgs *args = malloc(sizeof(FrogThreadArgs));
    if (!args) {
        perror("Errore nell'allocazione della memoria per FrogThreadArgs");
        exit(1);
    }
    args->frog = frog;
    
    args->granata = granata;
    args->proiettile = proiettile;
    
    // Crea il thread per la rana
    int ret = pthread_create(&frog->base.tid, NULL, frog_thread, (void *)args);
    if (ret != 0) {
        perror("Errore nella creazione del thread rana");
        exit(1);
    }
    // Il thread della rana è ora in esecuzione
}

void creaCroco(Crocodile croco[], int numCroco, int pipefd[2]) {
    srand(time(NULL));
    int alternanza = rand() % 2;
    for (int i = 0; i < numCroco; i++) {
        croco[i].base.id = i + 1;
        int riga = (i / 3) + 8;
        int col;
        int spazio = COLS / 3;
        
        if ((riga + alternanza) % 2 == 1) {
            col = (i % 3) * spazio;
            croco[i].direction = 1;
        } else {
            col = COLS - (i % 3) * spazio - spazio;
            croco[i].direction = 2;
        }
        croco[i].base.x = col;
        croco[i].base.y = riga;
        
        // Prepara gli argomenti per il thread del coccodrillo
        CrocoThreadArgs *args = malloc(sizeof(CrocoThreadArgs));
        if (!args) {
            perror("Errore nell'allocazione della memoria per CrocoThreadArgs");
            exit(1);
        }
        args->croco = &croco[i];
        args->pipefd[0] = pipefd[0];
        args->pipefd[1] = pipefd[1];
        
        int ret = pthread_create(&croco[i].base.tid, NULL, croco_thread, (void *)args);
        if (ret != 0) {
            perror("Errore nella creazione del thread coccodrillo");
            exit(1);
        }
    }
}

void creaProiettile(Entity *proiettile, int pipefd[2], int pipeEvent[2], int numCroco, Crocodile croco[], Entity granata[]) {
    srand(time(NULL));
    // Seleziona un coccodrillo casuale (indice 0-based)
    int index = rand() % numCroco;
    
    // Imposta la posizione del proiettile in base al coccodrillo scelto
    proiettile->x = croco[index].base.x;
    proiettile->y = croco[index].base.y;
    
    // Prepara gli argomenti per il thread del proiettile
    ProiettileThreadArgs *args = malloc(sizeof(ProiettileThreadArgs));
    if (!args) {
        perror("Errore nell'allocazione della memoria per ProiettileThreadArgs");
        exit(1);
    }
    args->proiettile = proiettile;
    args->pipefd[0] = pipefd[0];
    args->pipefd[1] = pipefd[1];
    args->pipeEvent[0] = pipeEvent[0];
    args->pipeEvent[1] = pipeEvent[1];
    args->numCroco = numCroco;
    args->croco = &croco[index];
    args->granata = granata;
    
    int ret = pthread_create(&proiettile->tid, NULL, proiettile_thread, (void *)args);
    if (ret != 0) {
        perror("Errore nella creazione del thread proiettile");
        exit(1);
    }
}

void creaGranata(Entity granata[], int pipefd, Frog frog, Entity *proiettile) {
    // Imposta la posizione della granata in base a quella della rana
    // Qui si usa granata[0] (oppure l'elemento in base all'id) come riferimento
    granata->x = frog.base.x + (granata->id == 60 ? -1 : 3);
    granata->y = frog.base.y;
    granata->sprite = spriteGranata;
    
    // Prepara gli argomenti per il thread della granata
    GranataThreadArgs *args = malloc(sizeof(GranataThreadArgs));
    if (!args) {
        perror("Errore nell'allocazione della memoria per GranataThreadArgs");
        exit(1);
    }
    args->granata = granata;
    args->pipefd = pipefd;
    args->proiettile = proiettile;
    
    int ret = pthread_create(&granata->tid, NULL, granata_thread, (void *)args);
    if (ret != 0) {
        perror("Errore nella creazione del thread granata");
        exit(1);
    }
}
