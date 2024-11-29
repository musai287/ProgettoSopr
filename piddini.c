#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>

#include "piddini.h"

void creaPipe(int pipe_fd[2]) {
    if (pipe(pipe_fd) == -1) {
        perror("Errore nella creazione della pipe");
        exit(1);
    }
}

void creaRano(int pipe_fd[2], pid_t *pid_rana) {
    *pid_rana = fork();
    if (*pid_rana == -1) {
        perror("Errore nella creazione della prima fork");
        _exit(1);
    }

    if (*pid_rana == 0) {  // Processo figlio (rana)
        close(pipe_fd[0]);  // Chiudi il lato di lettura della pipe
        // Esegui altre operazioni nel processo figlio 'rana'
        write(pipe_fd[1], "Rana", 4);  // Esempio di scrittura nella pipe
        close(pipe_fd[1]);  // Chiudi il lato di scrittura della pipe
        _exit(0);  // Esci dal processo figlio
    }
}

void creaCroco(int pipe_fd[2], pid_t *pid_croco) {
    *pid_croco = fork();
    if (*pid_croco == -1) {
        perror("Errore nella creazione della seconda fork");
        _exit(1);
    }

    if (*pid_croco == 0) {  // Processo figlio (coccodrillo)
        close(pipe_fd[0]);  // Chiudi il lato di lettura della pipe
        // Esegui altre operazioni nel processo figlio 'coccodrillo'
        write(pipe_fd[1], "Croco", 5);  // Esempio di scrittura nella pipe
        close(pipe_fd[1]);  // Chiudi il lato di scrittura della pipe
        _exit(0);  // Esci dal processo figlio
    }
}