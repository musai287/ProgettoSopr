#pragma once
#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>



//void pidd(int pi[],pid_t a, pid_t b);
//void creaPipe(int pipe_fd[2]) ;
//void creaRano(int pipe_fd[2]) ; 
//void creaCroco(int pipe_fd[2]);

void creaPipe(int pipe_fd[2]);
void creaRano(int pipeRana[2], pid_t *pid_rana);
void creaCroco(int numCroco,int pipeCroco[2], pid_t *pid_croco);
