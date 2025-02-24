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
        //mvwprintw(gioco, croco[i].base.y+1, croco[i].base.x, "%d", croco[i].direction);
    }          
   }

   void processoProiettile(Entity *proiettile, int pipefd, int pipeEvent, Crocodile croco[], int numCroco, Entity granata[]) {
    Event evento;
    int delayMovimento = 2;
    srand(time(NULL) + proiettile->id); // Random seed unico per ogni processo
    sleep(rand() % delayMovimento);

    // Sceglie un coccodrillo iniziale casuale
    int indexCroco = rand() % numCroco+1;
     proiettile->x = croco[indexCroco].base.x;
     proiettile->y = croco[indexCroco].base.y;

    while (1) {
       
        if (proiettile->x >= COLS-1 || proiettile->x <= -6) {
            // Se il proiettile esce dallo schermo, cambia coccodrillo
            indexCroco = rand() % numCroco;
            proiettile->x = croco[indexCroco].base.x;
            proiettile->y = croco[indexCroco].base.y;
            // waitpid(proiettile->pid, NULL, 0);
            // _exit(0);
        }
        for(int i = 0; i < 2; i++){
            if(proiettile->x == granata[i].x && proiettile->y == granata[i].y){
                proiettile->x = COLS;
            }
        }
        

        // Muove il proiettile nella direzione del coccodrillo selezionato
        if (croco[indexCroco].direction == 1) {
            entity_move(proiettile, 1, 0);
        } else {
            entity_move(proiettile, -1, 0);
        }

        // Scrive nella pipe lo stato aggiornato
        write(pipefd, proiettile, sizeof(Entity));
        usleep(100000);
        
    }
}
