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

    
            
	while(1) {
        
        if (read(pipeEvent, &evento, sizeof(Event)) > 0) {
            if (evento.tipo == 2) {
                // La rana è sopra un coccodrillo e deve seguirlo
                // In questo caso, non sappiamo direttamente la posizione del coccodrillo, 
                // ma possiamo impostare una logica per spostare la rana
                // La rana può seguire il coccodrillo tramite una logica di movimento
                frog.base.x += evento.data;  // La rana segue il coccodrillo sulla X
            } else if (evento.tipo == 0) {
                // Tipo 0: La rana si sta muovendo in modo normale
                frog.base.x;  // Movimento normale
            }
        }

	int input = getch();
	//if(input == 'q'){
	//endwin();
	//exit(0);
	//}
	//ho dubbi su questa implementazione,non funziona ma penso sia dovuto al fatto che la pipe sia condivisa.
    
    
    
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