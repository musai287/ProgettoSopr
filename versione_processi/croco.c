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
    int delayMovimento = 2;
    srand(time(NULL)+croco->base.id); // Random seed unico per ogni processo
    sleep(rand() % delayMovimento); // Pausa randomica
    while (1) {
       
        if(croco->direction == 1){
            croco->base.entity_move(&croco->base, 1, 0);  
        }
        else{    
            croco->base.entity_move(&croco->base, -1, 0);
            
        } 
        if (croco->base.x > COLS +4) {
            croco->base.x = 1;
            usleep(rand() % delayMovimento); // Pausa randomica
        }
        if(croco->base.x <= -7){
            croco->base.x = COLS;
            usleep(rand() % delayMovimento); // Pausa randomica
            }
        write(pipefd, &croco->base, sizeof(Entity));
        usleep(200000); // Velocità del movimento
    }
}
void stampCocco(WINDOW *gioco, int numCroco, Crocodile croco[]) {
    for (int i = 0; i < numCroco; i++) {  
        if(croco[i].direction == 1){
            stampaEntity(gioco, &(croco[i].base));
        }
        if(croco[i].direction == 2){
            antiStampaEntity(gioco, &(croco[i].base));
        }
        mvwprintw(gioco, croco[i].base.y+1, croco[i].base.x, "%d", croco[i].base.id);
    }          
   }

