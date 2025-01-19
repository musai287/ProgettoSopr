#pragma once
#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>
#include <string.h>
#include <fcntl.h>
#include <ncurses.h>
#include "struct.h"

int ranaSuCrocoCheck(Frog *frog, Crocodile *croco, int numCroco);
int ranaSuCroco(Frog *frog, Crocodile *croco, int numCroco);
int ranaInFiume(Frog *frog, Crocodile *croco, int numCroco);
int ranaInTanaCheck(Frog *frog, Map *tana);
int ranaInTana(Frog *frog, Map *tana);