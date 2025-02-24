#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>

#include "struct.h"
#include "frog.h"

void processoRana(Frog frog,int pipe_fd,int pipeEvent, Entity granata[], Entity *proiettile){
    
    Event evento;   
    while(1){
        int input = movimento();

        // Calcola la posizione futura della rana
        int new_x = frog.base.x;
        int new_y = frog.base.y;

        switch (input) {
            case KEY_UP:
                new_y -= 1;
            break;
            case KEY_DOWN:
                new_y += 1;
            break;
            case KEY_LEFT:
                new_x -= 1;
            break;
            case KEY_RIGHT:
                new_x += 1;
            break;
            case ' ' :
            for (int i = 0; i < 2; i++) {
                creaGranata(&granata[i], pipe_fd, frog, proiettile);}
            break;
           
        }
        // Verifica che la nuova posizione sia entro i limiti dello schermo
        if (new_x >= 1 &&
            new_x + frog.base.sprite.larghezza <= COLS &&
            new_y >= 0 &&
            new_y + frog.base.sprite.lunghezza <= LINES-4) {
            frog.base.entity_move(&frog.base, new_x - frog.base.x, new_y - frog.base.y);
        }
        write(pipe_fd, &frog.base, sizeof(Entity));
   
        if (read(pipeEvent, &evento, sizeof(Event)) <= 0){continue;}
        if (read(pipeEvent, &evento, sizeof(Event)) > 0) {
            if (evento.tipo == 2) {
                frog.base.x = evento.data;
            }
            else 
            if (evento.tipo == 3) {
                frog.base.x = (COLS /2) - 3;
                frog.base.y = LINES - 5;
            } 
            // else if (evento.tipo == 4 || 
            //          evento.tipo == 6 ||
            //          evento.tipo == 8 ||
            //          evento.tipo == 3){ 
            //     frog.base.x = (COLS /2) - 3;
            //     frog.base.y = LINES - 5;
            // }
        }
    }
}

void processoGranata(Entity *granata, int pipefd, Entity *proiettile){
    while(1){
        
        if(granata->id == 61){
                entity_move(granata, 1, 0);
                if(granata->x > COLS-1){
                    
                    waitpid(granata->pid, NULL,0);
                    granata->pid = 0;
                    _exit(0);
                }
            }
            
        else if(granata->id == 60){
                entity_move(granata, -1, 0);
                if(granata->x < 0){ 

                    waitpid(granata->pid, NULL,0);
                    _exit(0);
                }
            }
        else if(granata ->x == proiettile->x && granata->y == proiettile->y){
            granata ->x = COLS;
        }
        
        write(pipefd, granata, sizeof(Entity));
        usleep(100000);
    }
}

int movimento(){
    int input = getch();
    return input;
}