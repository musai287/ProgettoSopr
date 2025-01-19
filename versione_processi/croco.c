#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>

#include "struct.h"
#include "croco.h"

void processoCroco(Crocodile *croco,int pipefd) {

    srand(time(NULL)+croco->base.id); // Random seed unico per ogni processo
    sleep(rand() % 3 + 1);
    while (1) {
       
        if((croco->base.y) % 2 == 1){
            croco->base.entity_move(&croco->base, 1, 0);    
        }
        else{    
            croco->base.entity_move(&croco->base, -1, 0);
            
        } 
        if (croco->base.x > COLS) {
            croco->base.x = 1;
            sleep(rand() % 3 + 1); // Pausa randomica
        }
        if(croco->base.x <= 0){
            croco->base.x = COLS;
            sleep(rand() % 3 + 1); // Pausa randomica
            }
        write(pipefd, &croco->base, sizeof(Entity));
        usleep(200000); // Velocità del movimento
    }
}
void stampCocco(WINDOW *gioco, int numCroco, Crocodile croco[]) {
    for (int i = 0; i < numCroco; i++) {
       
        stampaEntity(gioco, &(croco[i].base));
     mvwprintw(gioco, croco[i].base.y+1, croco[i].base.x, "%d", croco[i].direction);
    }          
   }

