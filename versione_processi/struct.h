#pragma once

#define DELAY 1000000 //0.001
#define DELAYCLOSED 2000000 //2 secondi
typedef struct Fin {
    int height;
    int width;
    int starty;
    int startx;
} Fin;
typedef struct MesPos{
    int tipo;
	int x, y;
} MesPos;  //la struttura serve sia per la posizione dei "personaggi" e per la comunicazione nella pipe
extern WINDOW *vita, *gioco;
extern Fin fin1; 
extern Fin fin2;
extern MesPos croco;
extern MesPos rana;

void initFin();
void initSCroco();
void initSRana();