#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>

#include "struct.h"
#include "frog.h"

void rano(int pipe_fd){
	initSRana();
	while(1) {

	int input = getch();
	//if(input == 'q'){
	//endwin();
	//exit(0);
	//}
	//ho dubbi su questa implementazione,non funziona ma penso sia dovuto al fatto che la pipe sia condivisa.

	switch (input) {
		case KEY_UP:
		rana.x = (rana.x > 1) ? rana.x -1: rana.x;
		break;
		case KEY_DOWN:
		rana.x = (rana.x < LINES-5) ? rana.x +1 : rana.x ;
		break;
		case KEY_LEFT:
		rana.y = (rana.y > 1 ) ? rana.y -1: rana.y;
		break;
		case KEY_RIGHT:
		rana.y = (rana.y < COLS -2 ) ? rana.y +1 : rana.y;
		break;
		}//frecce direzionali per muovere
		write(pipe_fd, &rana, sizeof(MesPos));
	}  
}
void stampRana(int pipefd){
	mvwprintw(gioco,rana.x-1, rana.y, "___");
	mvwprintw(gioco,rana.x, rana.y,   "|_|"); //simbolo usato per rappresentare il personaggio
  	attron(A_REVERSE);
}