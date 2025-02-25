#include <ncurses.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include "struct.h"
#include "frog.h"
#include "croco.h"
#include "funzionamento.h"

int main() {
    initscr();
    noecho();
    curs_set(FALSE);
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    resize_term(23, 50);

    start_color();
    use_default_colors();
    init_pair(1, COLOR_GREEN, COLOR_BLUE);
    init_pair(2, COLOR_BLACK, COLOR_BLUE);  
    init_pair(3, COLOR_BLACK, COLOR_YELLOW);
    init_pair(4, COLOR_BLACK, COLOR_GREEN);
    init_pair(5, 2, -1);

    finestre();

    // Inizializziamo SharedData
    SharedData sd;
    pthread_mutex_init(&sd.lock, NULL);

    // Inizializza il buffer circolare
    pthread_mutex_init(&sd.buffer.mutex, NULL);
    pthread_cond_init(&sd.buffer.cond_non_vuoto, NULL);
    pthread_cond_init(&sd.buffer.cond_non_pieno, NULL);
    sd.buffer.head = 0;
    sd.buffer.tail = 0;
    sd.buffer.count = 0;

    sd.numCroco = 24;
    sd.gameOver = 0;
    sd.manche = 1;
    sd.punteggio = 0;

    // Inizializza la rana (id=1)
    sd.frog = initFrog();
    sd.frog.base.sd = &sd;
    // Inizializza i coccodrilli => ID = (i + 2)
    for(int i = 0; i < sd.numCroco; i++) {
        sd.croco[i] = initCrocodile();
        sd.croco[i].base.id = i + 2; 
    }

    // Disposizione alternata
    srand(time(NULL));
    int alternanza = rand() % 2;
    for (int i = 0; i < sd.numCroco; i++) {
        int riga = (i / 3) + 8;
        int col;
        int spazio = COLS / 3;
        if ((riga + alternanza) % 2 == 1) {
            col = (i % 3) * spazio;
            sd.croco[i].direction = 1;  // va a destra
        } else {
            col = COLS - (i % 3) * spazio - spazio;
            sd.croco[i].direction = 2;  // va a sinistra
        }
        sd.croco[i].base.x = col;
        sd.croco[i].base.y = riga;
        sd.croco[i].base.sd = &sd;
    }

    // Granate => id=60,61
    for (int i = 0; i < 2; i++) {
        sd.granata[i] = initGranata();
        sd.granata[i].id = 60 + i;
        sd.granata[i].sd = &sd; 
    }

    // Proiettile => id=30
    int index = rand() % sd.numCroco;

    // Imposta i dati del proiettile
    sd.proiettile = initProiettile();
    sd.proiettile.x = sd.croco[index].base.x;
    sd.proiettile.y = sd.croco[index].base.y;
    sd.proiettile.id = 30;
    sd.proiettile.sd = &sd;

    // Creiamo i thread
    pthread_t frogThread;
    pthread_create(&frogThread, NULL, threadRana, (void *)&sd);

    pthread_t granataThread[2];
    for (int i = 0; i < 2; i++) {
        pthread_create(&granataThread[i], NULL, threadGranata, (void *)&sd.granata[i]);
    }

    pthread_t crocoThread[sd.numCroco];
    for (int i = 0; i < sd.numCroco; i++) {
        pthread_create(&crocoThread[i], NULL, threadCroco, (void *)&sd.croco[i]);
    }

    pthread_t bulletThread;
    pthread_create(&bulletThread, NULL, threadProiettile, (void *)&sd.proiettile);

    // Esegue la logica di gioco
    funzionamento_gioco(&sd);

    // Se usciamo dalla funzione => gameOver=1 => i thread escono
    sd.gameOver = 1;

    // Join thread
    pthread_join(frogThread, NULL);
    for (int i = 0; i < 2; i++) {
        pthread_join(granataThread[i], NULL);
    }
    for (int i = 0; i < sd.numCroco; i++) {
        pthread_join(crocoThread[i], NULL);
    }
    pthread_join(bulletThread, NULL);

    endwin();
    return 0;
}
