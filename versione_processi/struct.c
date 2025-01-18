#include <ncurses.h>
#include "struct.h"
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>
/*nuova versione pardigma OOP*/

void entity_move(Entity *self, int dx, int dy) {
    self->x += dx;
    self->y += dy;
}

void antiStampaEntity(WINDOW *win, Entity *entity) {
    for (int i = 0; i < entity->sprite.larghezza; i++) {
        for (int j = entity->sprite.lunghezza -1; j > 0; j--) {
            // Accedi ai pixel invertendo l'ordine lungo la dimensione orizzontale
            mvwprintw(win, entity->y + j, entity->x + i, 
                      "%c", entity->sprite.pixels[i][j]);
        }
    }
}

void stampaEntity(WINDOW *win, Entity *entity){
    for(int i = 0; i < entity->sprite.larghezza; i++){
        for(int j = 0; j < entity->sprite.lunghezza; j++){
            mvwprintw(win, entity->y + j, entity->x + i, "%c", entity->sprite.pixels[i][j]);
        } 
    }
}
Sprite spriteRana ={
    3,
    1,
    {"|","_","|"}
};

Sprite spriteCrocodile = {
    6,
    1,
    {"[","=","=","=","]","="}
    };


Frog initFrog() {
    Frog frog;
    frog.base.x = 10;
    frog.base.y = 10;
    frog.base.sprite = spriteRana;
    frog.base.id = 0;
    frog.base.entity_move = entity_move;
    frog.base.pid;
    frog.lives = 3;

    return frog;
}

Crocodile initCrocodile() {
    Crocodile croco;
    croco.base.x ;
    croco.base.y ;
    croco.base.sprite = spriteCrocodile;
    croco.base.id = 1;
    croco.base.entity_move = entity_move;
    croco.base.pid;
    croco.direction;
    return croco;
}
WINDOW *vita, *gioco;
Fin fin1;
Fin fin2;
void initFin() {
    // Inizializza le variabili globali dopo aver chiamato initscr()
    fin1 = (Fin){3, COLS, 0, 0};
    fin2 = (Fin){LINES - 3, COLS, 3, 0};
}
/*vecchia versione*/
//WINDOW *vita, *gioco;
//Fin fin1;
//Fin fin2;
//MesPos croco;
//MesPos rana;
//
//void initFin() {
//    // Inizializza le variabili globali dopo aver chiamato initscr()
//    fin1 = (Fin){3, COLS, 0, 0};
//    fin2 = (Fin){LINES - 3, COLS, 3, 0};
//}
//
//void initSCroco(){
//    // Inizializza le variabili globali per croco   
//    croco = (MesPos){1, 0, 1, 1};
//}
//
//void initSRana(){
//    // Inizializza le variabili globali per rana
//    rana = (MesPos){0, 0, LINES - 6, COLS - 2};
//}