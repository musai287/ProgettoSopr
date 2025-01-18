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
	
    Event evento;

    
 while(1){           
        if (read(pipeEvent, &evento, sizeof(Event)) > 0) {
            if (evento.tipo == 2) {
                if(evento.data == 1){
                    frog.base.entity_move(&frog.base, 1, 0);  
                    usleep(100);
                }
                if(evento.data == 2){
                    frog.base.entity_move(&frog.base, -1, 0);
                    usleep(100);
                }
            } 
            else if (evento.tipo == 0) {   
	  
            }
        }
    int input = getch();
    
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