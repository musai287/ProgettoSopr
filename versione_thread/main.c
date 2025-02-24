#include <ncurses.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include "struct.h"
#include "frog.h"
#include "croco.h"
#include "funzionamento.h"


int main(){
    initscr();
    noecho();
    curs_set(FALSE);
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);

    start_color();
    use_default_colors();
    init_pair(1, COLOR_GREEN, COLOR_BLUE);
    init_pair(2, COLOR_BLACK, COLOR_BLUE);  
    init_pair(3, COLOR_BLACK, COLOR_YELLOW);
    init_pair(4, COLOR_BLACK, COLOR_GREEN);
    init_pair(5, 2, -1);

    finestre(); // Inizializza le due finestre vita e gioco

    // Creiamo la struttura condivisa
    SharedData sd;
    pthread_mutex_init(&sd.lock, NULL);

    sd.numCroco = 24;
    sd.gameOver = 0;
    sd.manche = 1;
    sd.punteggio = 0;

    // Inizializziamo la rana
    sd.frog = initFrog();

    // Inizializziamo i coccodrilli
    for(int i = 0; i < sd.numCroco; i++){
        sd.croco[i] = initCrocodile();
        sd.croco[i].base.id = i + 1;
    }

    // Inizializziamo granate
    for(int i = 0; i < 2; i++){
        sd.granata[i] = initGranata();
        sd.granata[i].id = 60 + i;
    }

    // Inizializziamo il proiettile
    sd.proiettile = initProiettile();
    sd.proiettile.id = 30;

    // Creiamo i thread
    pthread_t frogThread;
    pthread_create(&frogThread, NULL, threadRana, (void *)&sd);

    // Creiamo i thread per le 2 granate (uno per granata[i], se vuoi)
    pthread_t granataThread[2];
    for (int i = 0; i < 2; i++) {
        pthread_create(&granataThread[i], NULL, threadGranata, (void *)&sd.granata[i]);
    }

    // Creiamo i thread per i coccodrilli
    pthread_t crocoThread[sd.numCroco];
    for(int i = 0; i < sd.numCroco; i++) {
        pthread_create(&crocoThread[i], NULL, threadCroco, (void *)&sd.croco[i]);
    }

    // Creiamo il thread del proiettile
    pthread_t bulletThread;
    pthread_create(&bulletThread, NULL, threadProiettile, (void *)&sd.proiettile);

    // Nel main facciamo girare la logica di gioco
    funzionamento_gioco(&sd);

    // Quando esce dal ciclo, settiamo gameOver
    sd.gameOver = 1;

    // Aspettiamo che i thread finiscano
    pthread_join(frogThread, NULL);
    for (int i = 0; i < 2; i++) {
        pthread_join(granataThread[i], NULL);
    }
    for(int i = 0; i < sd.numCroco; i++) {
        pthread_join(crocoThread[i], NULL);
    }
    pthread_join(bulletThread, NULL);

    endwin();
    return 0;
}
