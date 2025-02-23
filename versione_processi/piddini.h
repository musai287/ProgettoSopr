#pragma once
#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>

#include "struct.h"
#include "frog.h"
#include "croco.h"


//void pidd(int pi[],pid_t a, pid_t b);
//void creaPipe(int pipe_fd[2]) ;
//void creaRano(int pipe_fd[2]) ; 
//void creaCroco(int pipe_fd[2]);

void creaPipe(int pipe_fd[2]);
void creaRano(Frog frog,int pipefd[2], int pipeEvent[2]);
void creaCroco(Crocodile croco[], int numCroco,int pipefd[2]);
void creaProiettile(Entity *proiettile, int pipefd[2], int pipeEvent[2], int numCroco ,Crocodile crocodile[]);
void creaGranata(Entity granata[], int pipefd[2], int pipeEvent[2], Frog frog);