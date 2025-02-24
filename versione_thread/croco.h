#pragma once
#include <ncurses.h>
#include "struct.h"

void *threadCroco(void *arg);           // Sostituisce processoCroco()
void stampCocco(WINDOW *gioco, int numCroco, Crocodile croco[]);

void *threadProiettile(void *arg);      // Sostituisce processoProiettile()
