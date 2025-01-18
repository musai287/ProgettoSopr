#pragma once
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>
#define DELAY 1000000 //0.001
#define DELAYCLOSED 2000000 //2 secondi

/*Nuova versione delle strutture, paradigma OOP*/
typedef struct Sprite{
    short int larghezza;  // e` la x è la riga
    short int lunghezza; // e` la y è la colonna
    char pixels[10][10];
}Sprite;

typedef struct map{
    Sprite sprite;
    short int x, y;
}map;

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

void antiStampaEntity(WINDOW *win, Entity *entity);
void entity_move(Entity *self, short int x, short int y);
void stampaEntity(WINDOW *win, Entity *entity);
extern Sprite spriteRana;
extern Sprite spriteCrocodile;
extern Frog initFrog();
extern Crocodile initCrocodile();


typedef struct Fin {
    short int height;
    short int width;
    short int starty;
    short int startx;
} Fin;

extern WINDOW *vita, *gioco;
extern Fin fin1; 
extern Fin fin2;

void initFin();

/*vecchia versione*/

//typedef struct MesPos{
//    short int id;
//    short int event; 
//    short int x, y;
//} MesPos;  //la struttura serve sia per la posizione dei "personaggi" e per la comunicazione nella pipe
//
//typedef struct Fin {
//    short int height;
//    short int width;
//    short int starty;
//    short int startx;
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