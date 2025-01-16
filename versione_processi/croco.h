#pragma once
#include <ncurses.h>
#include "struct.h"

void processoCroco(Crocodile *croco,int nC,int pipefd, int id);
void stampCocco(WINDOW *gioco, int numCroco, Crocodile croco[]);
//void stampCocco(int numCroco,int pipefd,int positions[]);