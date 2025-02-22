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
    int pipefd[2];
    int pipeEvent[2];
    creaPipe(pipefd); 
    creaPipe(pipeEvent);
    setNonBlocking(pipefd[0]);
    setNonBlocking(pipeEvent[1]);
	initscr();
    resize_term(23, 50);
	noecho();
	curs_set(FALSE);
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE); /*Insieme a setNonBlocking rende le pipe non bloccanti*/
    start_color();
    use_default_colors();
    init_pair(1, COLOR_GREEN, COLOR_BLUE);
    init_pair(2, COLOR_BLACK, COLOR_BLUE);  
    init_pair(3, COLOR_BLACK, COLOR_YELLOW);
    init_pair(4, COLOR_BLACK, COLOR_GREEN);
    init_pair(5, 2, -1);
    finestre(); // Creazione delle finestre

    Frog frog = initFrog(); // Inizializza la rana
    //creaRano(frog, pipefd, pipeEvent); // Crea il processo rana

    Crocodile croco[numCroco]; // Inizializza il coccodrillo
    for(int i=0; i < numCroco; i++){
        croco[i] = initCrocodile();
        croco[i].base.id++; // Inizializza il coccodrillo
    }
    //  creaCroco(croco, numCroco,pipefd);

    Entity granata[2]; // Inizializza la granata
    for (int i=0; i <2; i++){
        granata[i] = initGranata();
        //granata[i].id++;
    }
    // creaGranata(granata, pipefd, pipeEvent, frog);

    Entity proiettile[numCroco]; // Inizializza il proiettile   
    for(int i=0; i < numCroco; i++){
        proiettile[i] = initProiettile();
        proiettile[i].id =30+i;
        proiettile[i].x = croco[i].base.x;
        proiettile[i].y = croco[i].base.y;   
    }
    
    // creaProiettile(proiettile, pipefd, pipeEvent, numCroco, croco);

    funzionamento_gioco(frog, croco, numCroco,proiettile, granata, pipefd, pipeEvent);
    
    close(pipefd[1]); 	
    close(pipeEvent[0]);
    

	kill(frog.base.pid,SIGKILL);
    waitpid(frog.base.pid, NULL, 0);
    for (int i =0; i < 2;i++){
        kill(granata[i].pid, SIGKILL);
        waitpid(granata[i].pid, NULL, 0);
    }
    for (int i = 0; i < numCroco; i++) {
        kill(croco[i].base.pid, SIGKILL);  
        waitpid(croco[i].base.pid, NULL, 0);
        kill(proiettile[i].pid, SIGKILL);
        waitpid(proiettile[i].pid, NULL, 0);
    }
    endwin();
    return 0;
}





