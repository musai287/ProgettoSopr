#pragma once
#include <ncurses.h>
#include "struct.h"
#include <string.h>
#include <fcntl.h>

#include "frog.h"
#include "croco.h"


void setNonBlocking(int pipefd);
void finestre(Fin *fin1, Fin *fin2);
void gestisci_vite(int vite, time_t start_time);
void funzionamento_gioco (Frog frog,Crocodile croco,int numCroco, int pipefd, int positions[]);