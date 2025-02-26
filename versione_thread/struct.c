#include <ncurses.h>
#include <pthread.h>
#include <stdlib.h>
#include "struct.h"

/* ---------- Globali (finestre) ---------- */
WINDOW *vita, *gioco;

/* ---------- Funzioni generiche ---------- */
void entity_move(Entity *self, short int dx, short int dy) {
    self->x += dx;
    self->y += dy;
}

void antiStampaEntity(WINDOW *win, Entity *entity) {
    for (int i = 0; i < entity->sprite.larghezza; i++) {
        for (int j = entity->sprite.lunghezza - 1; j >= 0; j--) {
            mvwprintw(win, entity->y, entity->x + 2 - i,
                      "%c", entity->sprite.pixels[i][j]);
        }
    }
}

void stampaEntity(WINDOW *win, Entity *entity) {
    for (int i = 0; i < entity->sprite.larghezza; i++) {
        for (int j = 0; j < entity->sprite.lunghezza; j++) {
            mvwprintw(win, entity->y + j, entity->x + i,
                      "%c", entity->sprite.pixels[i][j]);
        }
    }
}

void stampaMap(WINDOW *win, Map *map) {
    for (int j = 0; j < map->sprite.lunghezza; j++) {
        for (int i = 0; i < map->sprite.larghezza; i++) {
            mvwprintw(win, map->y + i, map->x + j, "%c", map->sprite.pixels[i][j]);
        }
    }
}

/* ---------- Sprite definiti ---------- */
Sprite spriteGranata = {
    1, 1, {"o"}
};

Sprite spriteProiettile = {
    1, 1, {"+"}
};

Sprite spriteTana = {
    5, 5,
    {
        {'_', '_', '_', '_', '_'},
        {'|', ' ', ' ', ' ', '|'},
        {'|', ' ', ' ', ' ', '|'},
        {'|', ' ', ' ', ' ', '|'},
        {'_', '_', '_', '_', '_'}
    }
};

Sprite spriteTanaChiusa = {
    5, 5,
    {
        {'_', '_', '_', '_', '_'},
        {'|', '\\', ' ', '/', '|'},
        {'|', ' ', 'X', ' ', '|'},
        {'|', '/', ' ', '\\', '|'},
        {'_', '_', '_', '_', '_'}
    }
};

Sprite spriteRana = {
    3, 1, {"|","_","|"}
};

Sprite spriteCrocodile = {
    6, 1, {"|","=","=","=","|","="}
};

/* ---------- Inizializzazioni entità ---------- */
Map initTana() {
    Map tana;
    tana.sprite = spriteTana;
    tana.x = 0;
    tana.y = 0;
    return tana;
}

Frog initFrog() {
    Frog frog;
    frog.base.x = (COLS / 2) - 3;
    frog.base.y = LINES - 5;
    frog.base.sprite = spriteRana;
    frog.base.id = 1;  // la rana ha id=1
    frog.base.entity_move = entity_move;
    frog.lives = 3;
    return frog;
}

Entity initGranata() {
    Entity granata;
    granata.x = 0;
    granata.y = 0;
    granata.id = 60;
    granata.sprite = spriteGranata;
    granata.entity_move = entity_move;
    return granata;
}

Entity initProiettile() {
    Entity proiettile;
    proiettile.x = 0;
    proiettile.y = 0;
    proiettile.id = 30;
    proiettile.sprite = spriteProiettile;
    proiettile.entity_move = entity_move;
    return proiettile;
}

Crocodile initCrocodile() {
    Crocodile croco;
    croco.base.x = 0;
    croco.base.y = 0;
    croco.base.id = 2; // di default, i+2 in main
    croco.base.sprite = spriteCrocodile;
    croco.base.entity_move = entity_move;
    croco.direction = 1;
    return croco;
}

/* ---------- Implementazione buffer circolare ---------- */
void produceMessaggio(BufferCircolare *bc, Messaggio msg) {
    while (true) {
        pthread_mutex_lock(&bc->mutex);

        // Se c'è spazio, inseriamo il messaggio e usciamo
        if (bc->count < BUFFER_SIZE) {
            bc->buffer[bc->head] = msg;
            bc->head = (bc->head + 1) % BUFFER_SIZE;
            bc->count++;
            pthread_mutex_unlock(&bc->mutex);
            return; // Uscita
        }

        pthread_mutex_unlock(&bc->mutex);
    
    }
}

Messaggio consumeMessaggio(BufferCircolare *bc) {
    Messaggio msg;

    while (true) {
        pthread_mutex_lock(&bc->mutex);

        if (bc->count > 0) {
            msg = bc->buffer[bc->tail];
            bc->tail = (bc->tail + 1) % BUFFER_SIZE;
            bc->count--;
            pthread_mutex_unlock(&bc->mutex);
            return msg;
        }

        pthread_mutex_unlock(&bc->mutex);
        
    }
}
