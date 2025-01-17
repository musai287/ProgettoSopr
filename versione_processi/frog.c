#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>

#include "struct.h"
#include "frog.h"

void processoRana(Frog frog,int pipe_fd,int pipeEvent){
	
    Entity event;
	while(1) {
	int input = getch();
	//if(input == 'q'){
	//endwin();
	//exit(0);
	//}
	//ho dubbi su questa implementazione,non funziona ma penso sia dovuto al fatto che la pipe sia condivisa.

    read(pipeEvent, &event, sizeof(Entity));
    frog.base = event;
    
    
	switch (input) {
        case KEY_UP:
            frog.base.entity_move(&frog.base, 0, -1);
            break;
        case KEY_DOWN:
            frog.base.entity_move(&frog.base, 0, 1);
            break;
        case KEY_LEFT:
            frog.base.entity_move(&frog.base, -1, 0);
            break;
        case KEY_RIGHT:
            frog.base.entity_move(&frog.base, 1, 0);    
            break;
		}
     write(pipe_fd, &frog.base, sizeof(Entity));
	}  
}