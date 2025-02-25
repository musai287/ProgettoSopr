#pragma once
#include <ncurses.h>
#include "struct.h"

void *threadCroco(void *arg);           // Sostituisce processoCroco()
void stampCocco(WINDOW *gioco, Crocodile croco[]);

void *threadProiettile(void *arg);      // Sostituisce processoProiettile()
