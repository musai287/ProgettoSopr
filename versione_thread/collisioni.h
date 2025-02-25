#pragma once
#include <ncurses.h>
#include "struct.h"

int ranaSuCrocoCheck(Frog *frog, Crocodile *croco, int numCroco);
int ranaSuCroco(Frog *frog, Crocodile *croco, int numCroco);
int ranaInFiume(Frog *frog, Crocodile *croco, int numCroco);
int ranaInTanaCheck(Frog *frog, Map *tana);
int ranaInTana(Frog *frog, Map *tana);
int ranaProiettileCheck(Frog *frog, Entity *proiettile);
int ranaProiettile(Frog *frog, Entity *proiettile);
int proiettileFuori(Entity *proiettile);
