#pragma once

#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>

#include "struct.h"

#define DELAY 1000000 //0.001
#define DELAYCLOSED 2000000 //2 secondi

void processoRana(Frog frog,int pipe_fd, int pipeEvent);
//void stampRana(int pipefd);
void processoGranata(Entity *granata, int pipefd, int pipeEvent, Frog frog);
void player(int pipeEvent);