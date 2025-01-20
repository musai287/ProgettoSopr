/*quello che viene sparato dalla rana  dev'essere in realtà un array da due elmenti
 *usare l'id Entity.id per capire se va a sinistra o a destra.
 *quello del coccodrillo ha bisogno di un puntatore a processo coccodrillo per copiare direction*/

#include "proiettili.h"

void processoGranata(Entity granata, int pipefd, int pipeEvent, Frog frog){
    while(1){
        entity_move(&granata, 1, 0);
        write(pipefd, &granata, sizeof(Entity));
        usleep(100000);
        int input = getch();
        if(input == ){
            granata.x = frog.base.x;
            granata.y = frog.base.y;
    }
        write(pipefd, &granata, sizeof(Entity));

}
}
void processoProiettile(Entity proiettile, int pipefd, int pipeEvent, Crocodile crocodile){
    while(1){
        if(crocodile.direction == 1){
            entity_move(&proiettile, 1, 0);
            write(pipefd, &proiettile, sizeof(Entity));     
            usleep(100000);
        }else{
            entity_move(&proiettile, -1, 0);
            write(pipefd, &proiettile, sizeof(Entity));
            usleep(100000);
        }
    }
}
