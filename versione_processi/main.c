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
    int numCroco = 24;
	pid_t pid_rana;
    pid_t pid_croco[numCroco];
    int pipeRana[2], pipeCroco[2]; 
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
    creaPipe(pipeRana);  
    creaPipe(pipeCroco); 
    setNonBlocking(pipeRana[0]);
    setNonBlocking(pipeCroco[0]);
    initFin();
    finestre(&fin1, &fin2); // Creazione delle finestre
    
    // Crea il primo e il secondo processo
    creaRano(pipeRana, &pid_rana);
    creaCroco(numCroco,pipeCroco, pid_croco);
    
    close(pipeRana[1]); 		
    close(pipeCroco[1]);
    int positions[numCroco]; // Posizioni dei coccodrilli
    memset(positions, 0, sizeof(positions));
	funzionamento_gioco(numCroco, pipeRana[0],pipeCroco[0],positions); //richiamo la funzione padre 
	
	kill(pid_rana,1);
    for (int i = 0; i < numCroco; i++) {
    kill(pid_croco[i], SIGKILL);  // Uccidi ogni processo
}
	endwin();
	return 0;
}





