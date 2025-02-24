#pragma once
#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

#include "struct.h"
#include "frog.h"
#include "croco.h"

// Funzioni di creazione utilizzando thread
void creaPipe(int pipe_fd[2]);
void creaRano(Frog *frog, int pipefd[2], int pipeEvent[2], Entity granata[], Entity *proiettile);
void creaCroco(Crocodile croco[], int numCroco, int pipefd[2]);
void creaProiettile(Entity *proiettile, int pipefd[2], int pipeEvent[2], int numCroco, Crocodile crocodile[], Entity granata[]);
void creaGranata(Entity granata[], int pipefd, Frog frog, Entity *proiettile);
