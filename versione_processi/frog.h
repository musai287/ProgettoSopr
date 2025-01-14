#pragma once

#include <ncurses.h>
#include "struct.h"

#define DELAY 1000000 //0.001
#define DELAYCLOSED 2000000 //2 secondi

void processoRana(Frog frog,int pipe_fd);
//void stampRana(int pipefd);