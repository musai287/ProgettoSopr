#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>

#include "struct.h"
#include "funzionamento.h"


void funzionamento_gioco (int pipe_fd){
	MesPos message                         ;
	MesPos rana	    ={0, LINES -1, COLS -1};
	MesPos croco	={1,0,0}               ;

	while(1){
		if (read(pipe_fd, &message, sizeof(MesPos)) > 0) {
			if (message.tipo == 0) {
                rana = message;
			}
            else if (message.tipo == 1){
                croco = message;
            }
			//questo controllo if  else if serve per capire chi sta usando il buffer
			clear();
            attron(COLOR_PAIR(1))            ;
			mvprintw(rana.x, rana.y, "#")  ; //simbolo usato per rappresentare il personaggio
		    attron(COLOR_PAIR(2))            ;
            mvprintw(croco.x, croco.y, "$"); //simbolo usato per rappresentare il personaggio
			refresh();

			if (rana.x == croco.x && rana.y == croco.y){
				mvprintw(LINES / 2, COLS / 2 -5, "preso");
				refresh()                                ;
				usleep(DELAYCLOSED)                      ;
				endwin()                                 ;
				exit(0)                                  ;
			} //win condition
		}
	}
}