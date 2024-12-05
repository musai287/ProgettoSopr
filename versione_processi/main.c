#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>

#include "struct.h"
#include "frog.h"
#include "croco.h"
#include "funzionamento.h"
#include "piddini.h"

int main(){
	pid_t pid_rana, pid_croco;
    int pipe_fd[2]; 

	initscr();
	noecho();
	curs_set(FALSE);
    keypad(stdscr, TRUE);
    start_color();
    init_pair(1, COLOR_BLACK, COLOR_GREEN);
    init_pair(2, COLOR_YELLOW, COLOR_BLUE);  
    init_pair(3, COLOR_BLACK, COLOR_YELLOW);
    init_pair(4, COLOR_BLACK, COLOR_GREEN);
    creaPipe(pipe_fd);  // Crea la pipe
    
    initFin();
    finestre(&fin1, &fin2); // Creazione delle finestre
    
    // Crea il primo e il secondo processo
    creaRano(pipe_fd, &pid_rana);
    creaCroco(pipe_fd, &pid_croco);
    
    close(pipe_fd[1]); 
			
	funzionamento_gioco(pipe_fd[0]); //richiamo la funzione padre 
	
	kill(pid_rana,1);
	kill(pid_croco,1);
	endwin();
	return 0;
}





