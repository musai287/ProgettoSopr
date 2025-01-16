#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>

#include "piddini.h"
#include "frog.h"
#include "croco.h"
#include "struct.h"

void creaPipe(int pipe_fd[2]) {
    if (pipe(pipe_fd) == -1) {
        perror("Errore nella creazione della pipe");
        exit(1);
    }
}

void creaRano(Frog frog,int pipefd[2]) {
    frog.base.pid = fork();
    if (frog.base.pid == -1) {
        perror("Errore nella creazione della prima fork");
        _exit(1);
    }

    if (frog.base.pid == 0) {  // Processo figlio (rana)
        close(pipefd[0]);  // Chiudi il lato di lettura della pipe
        // Esegui altre operazioni nel processo figlio 'rana'
        processoRana(frog, pipefd[1]);  // Esempio di scrittura nella pipe
        close(pipefd[1]);  // Chiudi il lato di scrittura della pipe
        _exit(0);  // Esci dal processo figlio
    }
}

void creaCroco(Crocodile croco[], int numCroco,int pipefd[2]) {
    
    for(int i = 0; i <numCroco; i++) {
        croco[i].base.id = i+1;
       int riga = i / 3 + 1;
        int col;

        if (riga % 2 == 1) {
            col = (i % 3) * 10;
        } else {
            col = COLS - (i % 3) * 10 - 10;
            
        }
        croco[i].direction = 1;
        croco[i].base.x = col;
        croco[i].base.y = riga;
    


        if ((croco[i].base.pid = fork())) {
            close(pipefd[0]); // Chiudi il lato di lettura per i figli
            processoCroco(&croco[i], numCroco, pipefd[1], i);
            _exit(0);
        }
    }
}