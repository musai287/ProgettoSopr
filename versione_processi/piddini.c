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

void creaRano(Frog frog,int pipefd[2], pid_t *pid_rana) {
    *pid_rana = fork();
    if (*pid_rana == -1) {
        perror("Errore nella creazione della prima fork");
        _exit(1);
    }

    if (*pid_rana == 0) {  // Processo figlio (rana)
        close(pipefd[0]);  // Chiudi il lato di lettura della pipe
        // Esegui altre operazioni nel processo figlio 'rana'
        processoRana(frog, pipefd[1]);  // Esempio di scrittura nella pipe
        close(pipefd[1]);  // Chiudi il lato di scrittura della pipe
        _exit(0);  // Esci dal processo figlio
    }
}

void creaCroco(Crocodile croco, int numCroco,int pipefd[2], pid_t *pid_croco) {
    
    for(int i = 1; i <numCroco+1; i++) {
        if ((pid_croco[i] = fork()) == 0) {
            close(pipefd[0]); // Chiudi il lato di lettura per i figli
            processoCroco(croco, i, pipefd[1], i);
            _exit(0);
        }
    }
}