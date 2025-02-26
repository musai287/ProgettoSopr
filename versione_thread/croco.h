#pragma once
#include <ncurses.h>
#include "struct.h"

void *threadCroco(void *arg);           
void stampCocco(WINDOW *gioco, Crocodile croco[]);

void *threadProiettile(void *arg);
