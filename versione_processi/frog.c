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


 while(1){
    int input = getch();

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
        }

        // Verifica che la nuova posizione sia entro i limiti dello schermo
        if (new_x >= 0 && new_x + frog.base.sprite.larghezza <= COLS &&
            new_y >= 0 && new_y + frog.base.sprite.lunghezza <= LINES-4) {
            frog.base.entity_move(&frog.base, new_x - frog.base.x, new_y - frog.base.y);
        }
        write(pipe_fd, &frog.base, sizeof(Entity));


        if (read(pipeEvent, &evento, sizeof(Event)) <= 0){continue;}
        if (read(pipeEvent, &evento, sizeof(Event)) > 0) {
            if (evento.tipo == 2) {
                frog.base.x = evento.data;

            }
            else if (evento.tipo == 3) {
                frog.base.x = (COLS /2) - 3;
                frog.base.y = LINES - 5;

            }
        }
    }
}