#pragma once
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>
#define DELAY 1000000 //0.001
#define DELAYCLOSED 2000000 //2 secondi

/*Nuova versione delle strutture, paradigma OOP*/
typedef struct Sprite{
    short int larghezza;  // e` la x è la riga
    short int lunghezza;  // e` la y è la colonna
    char pixels[10][10];
}Sprite;

typedef struct Map{
    Sprite sprite;
    short int x, y;
}Map;

typedef struct Event{
    short int tipo;
    short int data;
}Event;



typedef struct Entity{
    short int x, y;
    Sprite sprite;
    short int id;
    void (*entity_move)(struct Entity *self, short int x, short int y);
    pid_t pid;
}Entity;

typedef struct Frog{
    Entity base;
    short int lives;
}Frog;

typedef struct Crocodile{
    Entity base;
    short int direction;
}Crocodile;

typedef struct Fin {
    short int height;
    short int width;
    short int starty;
    short int startx;
} Fin;

void antiStampaEntity(WINDOW *win, Entity *entity);
void entity_move(Entity *self, short int x, short int y);
void stampaEntity(WINDOW *win, Entity *entity);
void stampaMap(WINDOW *win, Map *map);
extern Sprite spriteGranata;
extern Sprite spriteProiettile;
extern Sprite spriteTana;
extern Sprite spriteRana;
extern Sprite spriteCrocodile;
extern Map initTana();
extern Frog initFrog();
extern Entity initGranata();
extern Entity initProiettile();
extern Crocodile initCrocodile();
void initFin();


extern WINDOW *vita, *gioco;
extern Fin fin1; 
extern Fin fin2;
