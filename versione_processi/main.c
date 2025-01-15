#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>
#include <string.h>

#include "struct.h"
#include "frog.h"
#include "croco.h"
#include "funzionamento.h"
#include "piddini.h"

int main(){
    int numCroco = 24; // Numero di coccodrilli
	pid_t pid_rana;
    pid_t pid_croco[numCroco];
    int pipefd[2];
	initscr();
	noecho();
	curs_set(FALSE);
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE); /*Insieme a setNonBlocking rende le pipe non bloccanti*/
    start_color();
    init_pair(1, COLOR_BLACK, COLOR_GREEN);
    init_pair(2, COLOR_YELLOW, COLOR_BLUE);  
    init_pair(3, COLOR_BLACK, COLOR_YELLOW);
    init_pair(4, COLOR_BLACK, COLOR_GREEN);
    creaPipe(pipefd);  
    setNonBlocking(pipefd[0]);
    initFin();
    finestre(&fin1, &fin2); // Creazione delle finestre
    
    Frog frog = initFrog(); // Inizializza la rana
    int ciao;
    Crocodile croco[numCroco]; // Inizializza il coccodrillo
    for(ciao=0; ciao < numCroco; ciao++){
        croco[ciao] = initCrocodile();
        croco[ciao].base.id++; // Inizializza il coccodrillo
    }
    
    
    // Crea il primo e il secondo processo
    creaRano(frog, pipefd, &pid_rana);
    creaCroco(croco, numCroco,pipefd, pid_croco);
    
    close(pipefd[1]); 	
    //int *positions =malloc(numCroco*sizeof(int)); // Posizioni dei coccodrilli

    int positions[numCroco]; // Posizioni dei coccodrilli
    memset(positions, 0, sizeof(positions)); //creazione dinamica 
	funzionamento_gioco(frog, croco, numCroco, pipefd[0],positions); //richiamo la funzione padre 
	
	kill(pid_rana,1);
    for (int i = 0; i < numCroco; i++) {
    kill(pid_croco[i], SIGKILL);  // Uccidi ogni processo
}
	endwin();
	return 0;
}





