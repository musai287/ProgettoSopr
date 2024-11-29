#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>

#include "piddini.h"
/*

void pidd(int pip[],pid_t pid_rana, pid_t pid_croco){
    int pipe_fd[2]; //creo la pipe
	pipe(pipe_fd);

	rano = fork();  //creo una prima fork 
		if (rano == 0){

		close(pipe_fd[0]);
		rano(pipe_fd[1]);
    	exit(0);	
	}	

	pid_croco = fork(); //ed una seconda fork 

	if(pid_croco == 0){

	close(pipe_fd[0]);
	pid_croco(pipe_fd[1]);
	exit(0);
	}

	close(pipe_fd[1]);
	
}

////////////////////////////////////////////
void creaPipe(int pipe_fd[2]) {
    if (pipe(pipe_fd) == -1) {
        perror("Errore nella creazione della pipe");
        exit(1);
    }
}

void creaRano(int pipe_fd[2]) {
    pid_t pid_rana = fork();
    if (pid_rana == -1) {
        perror("Errore nella creazione della prima fork");
        _exit(1);
    }
    
    if (rano == 0) {
        close(pipe_fd[0]);  // Chiudi il lato di lettura della pipe
        pid_rana(pipe_fd[1]);    // Chiama la funzione 'rano'
        //_exit(0);             // Esci dal primo processo
    }
}

void creaCroco(int pipe_fd[2]) {
    pid_t pid_croco = fork();
    if (pid_croco == -1) {
        perror("Errore nella creazione della seconda fork");
        _exit(1);
    }
    
    if (pid_croco == 0) {
        close(pipe_fd[0]);  // Chiudi il lato di lettura della pipe
        pid_croco(pipe_fd[1]);   // Chiama la funzione 'pid_croco'
        //_exit(0);             // Esci dal secondo processo
    }
}
*/
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