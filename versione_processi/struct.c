#include <ncurses.h>
#include "struct.h"

/*nuova versione pardigma OOP*/

void entity_move(Entity *self, int x, int y) {
    self->x += x;
    self->y += y;
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
    frog.lives = 3;
    return frog;
}

Crocodile initCrocodile() {
    Crocodile crocodile;
    crocodile.base.x = 2;
    crocodile.base.y = 2;
    crocodile.base.sprite = spriteCrocodile;
    crocodile.base.id = 1;
    crocodile.base.entity_move = entity_move;
    crocodile.direction;
    return crocodile;
}
WINDOW *vita, *gioco;
Fin fin1;
Fin fin2;
void initFin() {
    // Inizializza le variabili globali dopo aver chiamato initscr()
    fin1 = (Fin){3, COLS, 0, 0};
    fin2 = (Fin){LINES - 3, COLS, 3, 0};
}


//void initSprite(Sprite *sprite, int larghezza, int lunghezza, char **pixels){
//    sprite->larghezza = larghezza;
//    sprite->lunghezza = lunghezza;
//
//    sprite->pixels = malloc(lunghezza * sizeof(char *pattern));
//    for(int i =0; i < lunghezza; i++){
//        sprite->pixels[i] = malloc(larghezza * sizeof(char));
//    }
//    
//    for(int j = 0; j < larghezza; j++){
//        memcpy(sprite->pixels[i],&pattern[i* larghezza], larghezza);
//        }
//    }
//
//void FreeSprite(Sprite *sprite){
//    for(int i = 0; i < sprite->lunghezza; i++){
//        free(sprite->pixels[i]);
//    }
//    free(sprite->pixels);
//}



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