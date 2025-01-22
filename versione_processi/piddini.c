#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>

#include "piddini.h"
// #include "frog.h"
// #include "croco.h"
// #include "struct.h"
// #include "proiettili.h"

void creaPipe(int pipe_fd[2]) {
    if (pipe(pipe_fd) == -1) {
        perror("Errore nella creazione della pipe");
        exit(1);
    }
}

void creaRano(Frog frog,int pipefd[2], int pipeEvent[2]) {
    frog.base.pid = fork();
    if (frog.base.pid == -1) {
        perror("Errore nella creazione della prima fork");
        _exit(1);
    }

    if (frog.base.pid == 0) {  // Processo figlio (rana)
        close(pipefd[0]);  // Chiudi il lato di lettura della pipe
        // Esegui altre operazioni nel processo figlio 'rana'
        processoRana(frog, pipefd[1], pipeEvent[0]);  // Esempio di scrittura nella pipe
        close(pipeEvent[1]);  // Chiudi il lato di scrittura della pipe
        close(pipefd[1]);
        close(pipeEvent[0]);
        _exit(0);  // Esci dal processo figlio
    }
}

void creaCroco(Crocodile croco[], int numCroco,int pipefd[2]) {
    srand(time(NULL));
    int alternanza = rand() % 2;
    for(int i = 0; i <numCroco; i++) {
        croco[i].base.id = i+1;
        int riga = (i / 3) + 8;
        int col;
        int spazio = COLS / 3;

        if ((riga+alternanza) % 2 == 1) {
            col = (i % 3) * spazio;
            croco[i].direction = 1;

        } else {
            col = COLS - (i % 3) * spazio - spazio;
            croco[i].direction = 2;
        }
        croco[i].base.x = col;
        croco[i].base.y = riga;

        if ((croco[i].base.pid = fork())) {
            close(pipefd[0]); // Chiudi il lato di lettura per i figli
            processoCroco(&croco[i], pipefd[1]);
            _exit(0);
        }
    }
}

void creaProiettile(Entity proiettile[],int pipefd[2], int pipeEvent[2],int numCroco, Crocodile croco[]) {
    srand(time(NULL));

    // Seleziona un coccodrillo casuale
   for (int i = 0; i < numCroco; i++) {

    // Imposta i dati del proiettile
    
    proiettile[i].x = croco[i].base.x;
    proiettile[i].y = croco[i].base.y;

    // Crea un processo figlio per il proiettile
    proiettile[i].pid = fork();
    if (proiettile[i].pid == -1) {
        perror("Errore nella creazione della fork");
        _exit(1);
    }
    if (proiettile[i].pid == 0) {  // Processo figlio (proiettile)
        close(pipefd[0]);  // Chiudi il lato di lettura della pipe
        // Esegui altre operazioni nel processo figlio proiettile
        processoProiettile(&proiettile[i], pipefd[1], pipeEvent[0], &croco[i]);
        close(pipeEvent[1]);  // Chiudi il lato di scrittura della pipe
        close(pipeEvent[0]);  // Chiudi il lato di scrittura della pipe
        close(pipefd[1]);
        _exit(0);  // Esci dal processo figlio
        }
    }
}

void creaGranata(Entity granata[],int pipefd[2], int pipeEvent[2],Frog frog) {
    for (int i=0; i <2; i++){
        granata[0].id = 60;
        granata[1].id = 61;
        granata[0].x = frog.base.x - 1;
        granata[0].y = frog.base.y;    
        granata[1].x = frog.base.x + 3;
        granata[1].y = frog.base.y;
        granata[0].sprite = spriteGranata;
        granata[1].sprite = spriteGranata;
        granata[i].pid = fork();
        if (granata[i].pid == -1) {
            perror("Errore nella creazione della fork");
            _exit(1);
        }if (granata[i].pid == 0) {  // Processo figlio granata
            close(pipefd[0]);  // Chiudi il lato di lettura della pipe
        // Esegui altre operazioni nel processo figlio proiettile
            processoGranata(&granata[i], pipefd[1], pipeEvent[0], frog);  // Esempio di scrittura nella pipe
            close(pipeEvent[1]);  // Chiudi il lato di scrittura della pipe
            close(pipefd[1]);
            close(pipeEvent[0]);
            _exit(0);  // Esci dal processo figlio
    }
    }
}

