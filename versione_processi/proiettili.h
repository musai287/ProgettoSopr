#pragma once
#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>

#include "struct.h"

void processoGranata(Entity *granata, int pipefd, int pipeEvent, Frog frog);
void processoProiettile(Entity *proiettile, int pipefd, int pipeEvents, Crocodile *croco);