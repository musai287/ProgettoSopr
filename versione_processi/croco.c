#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>

#include "struct.h"
#include "croco.h"

void processoCroco(Crocodile croco,int nC,int pipefd, int y) {
      // Inizia in alto a sinistra
    
    srand(time(NULL) + nC); // Random seed unico per ogni processo
    sleep(rand() % 3 + 1);
    
    while (1) {
       
            if((croco.base.y) % 2 == 1){
                croco.base.x++;
            }
            else{    
                croco.base.x--;
            } 
        
        
        if (croco.base.x >= COLS) {
            // Scrivi un messaggio di uscita e resetta la posizione
            croco.base.x = 0;
            croco.base.event = 1; // Evento: uscito dallo schermo
            croco.base.id = nC;
            write(pipefd, &croco.base, sizeof(Entity));
            sleep(rand() % 3 + 1); // Pausa randomica
        }else{
            croco.base.event = 0; // Evento: in movimento
            croco.base.id = nC;
            write(pipefd, &croco.base, sizeof(Entity));
        }
         usleep(200000); // Velocità del movimento
    }
}
