#include <ncurses.h>
#include "struct.h"
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>
/*nuova versione pardigma OOP*/

void entity_move(Entity *self, short int dx, short int dy) {
    self->x += dx;
    self->y += dy;
}

void antiStampaEntity(WINDOW *win, Entity *entity) {
    for (int i = 0; i < entity->sprite.larghezza; i++) {
        for (int j = entity->sprite.lunghezza -1; j >= 0; j--) {
            // Accedi ai pixel invertendo l'ordine lungo la dimensione orizzontale
            mvwprintw(win, entity->y , entity->x +2 - i,
                      "%c", entity->sprite.pixels[i][j]);
        }
    }
}

void stampaEntity(WINDOW *win, Entity *entity){
    for(int i = 0; i < entity->sprite.larghezza; i++){
        for(int j = 0; j < entity->sprite.lunghezza; j++){
            mvwprintw(win, entity->y + j, entity->x + i,
                     "%c", entity->sprite.pixels[i][j]);
        }
    }
}
/*questa stampa è super giusta per le matrici finalmente le so fare*/
void stampaMap(WINDOW *win, Map *map){
    for(int j = 0; j < map->sprite.lunghezza; j++){
        for(int i = 0; i < map->sprite.larghezza; i++){
            mvwprintw(win, map->y + i, map->x + j, "%c", map->sprite.pixels[i][j]);
        }
    }
}
Sprite spriteGranata = {
    1,
    1,
    {"o"}
};
Sprite spriteProiettile = {
    1,
    1,
    {"+"}
};

Sprite spriteTana = {
    5,
    5,
    {
    {'_', '_', '_', '_', '_'},
    {'|', ' ', ' ', ' ', '|'},
    {'|', ' ', ' ', ' ', '|'},
    {'|', ' ', ' ', ' ', '|'},
    {'_', '_', '_', '_', '_'}
    }
};

Sprite spriteTanaChiusa = {
    5,
    5,
    {
    {'_', '_', '_', '_', '_'},
    {'|', '\\', ' ', '/', '|'},
    {'|', ' ', 'X', ' ', '|'},
    {'|', '/', ' ', '\\', '|'},
    {'_', '_', '_', '_', '_'}
    }
};

Sprite spriteRana ={
    3,
    1,
    {"|","_","|"}
};

Sprite spriteCrocodile = {
    6,
    1,
    {"|","=","=","=","|","="}
    };

Map initTana () {
    Map tana;
    tana.sprite = spriteTana;
    return tana;

};

Frog initFrog() {
    Frog frog;
    frog.base.x = (COLS /2) - 3;
    frog.base.y = LINES - 5;
    frog.base.sprite = spriteRana;
    frog.base.id = 0;
    frog.base.entity_move = entity_move;
    frog.lives = 3;
    return frog;
}
Entity initGranata() {
    Entity granata;
    granata.sprite = spriteGranata;
    granata.entity_move = entity_move;
    return granata;
}
Entity initProiettile() {
    Entity proiettile;
    proiettile.sprite = spriteProiettile;
    proiettile.entity_move = entity_move;
    return proiettile;
}
Crocodile initCrocodile() {
    Crocodile croco;
    croco.base.sprite = spriteCrocodile;
    croco.base.id = 1;
    croco.base.entity_move = entity_move;
    return croco;
}
WINDOW *vita, *gioco;
