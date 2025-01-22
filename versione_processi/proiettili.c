/*quello che viene sparato dalla rana  dev'essere in realtà un array da due elmenti
 *usare l'id Entity.id per capire se va a sinistra o a destra.
 *quello del coccodrillo ha bisogno di un puntatore a processo coccodrillo per copiare direction*/

#include "proiettili.h"
Event evento;
void processoGranata(Entity *granata, int pipefd, int pipeEvent, Frog frog){
    while(1){
        if (read(pipeEvent, &evento, sizeof(Event)) > 0) {
           if(evento.tipo == 5){

                if(granata->id == 60){
                    granata->x = frog.base.x - 1;
                    granata->y = frog.base.y;
                }
                else if(granata->id == 61){
                    granata->x = frog.base.x + 3;
                    granata->y = frog.base.y;
                }
            }
        }
        if(granata->id == 61){
            if(granata->x >= frog.base.x &&granata->x < COLS-3){
                entity_move(granata, 1, 0);
            }
        }
        else if(granata->id == 60){
            if(granata->x <= frog.base.x&&granata->x >2){
                entity_move(granata, -1, 0);
            }
        }
        write(pipefd, granata, sizeof(Entity));
        usleep(100000);
    }
}
void processoProiettile(Entity *proiettile, int pipefd, int pipeEvent, Crocodile *croco){
    int delayMovimento = 2;
    srand(time(NULL)+proiettile->id); // Random seed unico per ogni processo
    sleep(rand() % delayMovimento);
    while(1){
        if(croco->direction == 1){
            entity_move(proiettile, 1, 0);    
        }else{
            entity_move(proiettile, -1, 0);
        }
        if (proiettile->x > COLS) {
            proiettile->x = croco->base.x;
            usleep(rand() % delayMovimento); // Pausa randomica
        }
        if(proiettile->x <= -7){
            proiettile->x = croco->base.x;
            usleep(rand() % delayMovimento); // Pausa randomica
            }
        write(pipefd, proiettile, sizeof(Entity));
        usleep(100000);
    }
}
