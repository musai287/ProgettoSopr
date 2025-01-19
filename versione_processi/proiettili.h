#pragma once
#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>

#include "struct.h"

void processoGranata(Entity granata, int pipefd[2], int pipeEvent[2]);
void processoProiettile(Entity proiettile, int pipefd[2], int pipeEvent[2]);