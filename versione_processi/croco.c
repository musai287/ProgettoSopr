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

    srand(time(NULL)); // Random seed unico per ogni processo
    sleep(rand() % 3 + 1);
    while (1) {
       
        if((croco->base.y) % 2 == 1){
            croco->base.entity_move(&croco->base, 1, 0);
            croco->direction = 0;
        }
        else{    
            croco->base.entity_move(&croco->base, -1, 0);
            croco->direction = 1;
        } 
        
        
        if (croco->base.x >= COLS) {
            // Scrivi un messaggio di uscita e resetta la posizione
            //croco->base.event = 1; // Evento: uscito dallo schermo
            croco->base.x = 0;
           // write(pipefd, &croco->base, sizeof(Entity));
            sleep(rand() % 3 + 1); // Pausa randomica
        }
        if(croco->base.x <= 0){
            croco->base.x = COLS;
            //write(pipefd, &croco->base, sizeof(Entity));
            sleep(rand() % 3 + 1); // Pausa randomica
            }
        
        write(pipefd, &croco->base, sizeof(Entity));
         usleep(200000); // Velocità del movimento
    }
}
void stampCocco(WINDOW *gioco, int numCroco, Crocodile croco[]) {
    for (int i = 0; i < numCroco; i++) {
       
        stampaEntity(gioco, &(croco[i].base));
    }          
     //mvwprintw(gioco, croco[i].base.y+1, croco[i].base.x, "%d", croco[i].base.id);
   }

