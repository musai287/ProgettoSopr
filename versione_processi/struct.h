#pragma once
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>
#define DELAY 1000000 //0.001
#define DELAYCLOSED 2000000 //2 secondi

/*Nuova versione delle strutture, paradigma OOP*/
typedef struct Event{
    int tipo;
    int data;
}Event;




typedef struct Sprite{
    int larghezza;  // e` la x è la riga
    int lunghezza; // e` la y è la colonna
    char pixels[10][10];
}Sprite;

typedef struct Entity{
    int x, y;
    Sprite sprite;
    int id;
    
    void (*entity_move)(struct Entity *self, int x, int y);
    pid_t pid;
}Entity;

typedef struct Frog{
    Entity base;
    int lives;
}Frog;

typedef struct Crocodile{
    Entity base;
    int direction;
}Crocodile;

void antiStampaEntity(WINDOW *win, Entity *entity);
void entity_move(Entity *self, int x, int y);
void stampaEntity(WINDOW *win, Entity *entity);
extern Sprite spriteRana;
extern Sprite spriteCrocodile;
extern Frog initFrog();
extern Crocodile initCrocodile();


typedef struct Fin {
    int height;
    int width;
    int starty;
    int startx;
} Fin;

extern WINDOW *vita, *gioco;
extern Fin fin1; 
extern Fin fin2;

void initFin();

/*vecchia versione*/

//typedef struct MesPos{
//    int id;
//    int event; 
//    int x, y;
//} MesPos;  //la struttura serve sia per la posizione dei "personaggi" e per la comunicazione nella pipe
//
//typedef struct Fin {
//    int height;
//    int width;
//    int starty;
//    int startx;
//} Fin;
//
//extern WINDOW *vita, *gioco;
//extern Fin fin1; 
//extern Fin fin2;
//extern MesPos croco;
//extern MesPos rana;
//
//void initFin();
//void initSCroco();
//void initSRana();