#pragma once
#include <ncurses.h>
#include "struct.h"

void processoCroco(Crocodile *croco,int pipefd);
void stampCocco(WINDOW *gioco, int numCroco, Crocodile croco[]);
//void stampCocco(int numCroco,int pipefd,int positions[]);
// void processoProiettile(Entity *proiettile, int pipefd, int pipeEvents, Crocodile *croco);
void processoProiettile(Entity *proiettile, int pipefd, int pipeEvents, Crocodile *croco, int numCroco);
