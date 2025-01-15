#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>

#include "struct.h"
#include "croco.h"

void processoCroco(Crocodile *croco,int nC,int pipefd, int id) {

    srand(time(NULL) + croco->base.id); // Random seed unico per ogni processo
    sleep(rand() % 3 + 1);
    
    while (1) {
       
            if((croco->base.y) % 2 == 1){
                croco->base.entity_move(&croco->base, -1, 0);
            }
            else{    
                croco->base.entity_move(&croco->base, 1, 0);
            } 
        
        
        if (croco->base.x >= COLS) {
            // Scrivi un messaggio di uscita e resetta la posizione
            croco->base.x = 0;
            croco->base.event = 1; // Evento: uscito dallo schermo
            croco->base.id = id;
            write(pipefd, &croco->base, sizeof(Entity));
            sleep(rand() % 3 + 1); // Pausa randomica
        }else{
            croco->base.event = 0; // Evento: in movimento
            croco->base.id = id;
            write(pipefd, &croco->base, sizeof(Entity));
        }
         usleep(200000); // Velocità del movimento
    }
}

void aggiornaPosizioni(int numCroco, int positions[], Crocodile croco[]) {
    for (int i = 0; i < numCroco; i++) {
        int row = i / 3 + 1;
        int col;

        if (row % 2 == 1) {
            col = (i % 3) * 10;
        } else {
            col = COLS - (i % 3) * 10 - 10;
        }

        croco[i].base.x = col + positions[i];
        croco[i].base.y = row;
    }
}

void stampCocco(WINDOW *gioco, int numCroco, Crocodile croco[]) {
    for (int i = 0; i < numCroco; i++) {
        stampaEntity(gioco, &(croco[i].base));
    }
}
