#pragma once
#include <ncurses.h>
#include <string.h>
#include <fcntl.h>

#include "struct.h"
#include "frog.h"
#include "croco.h"
#include "collisioni.h"

void setNonBlocking(int pipefd);
void finestre();
void gestisci_vite(int vite, time_t start_time);
void funzionamento_gioco (Frog frog,Crocodile croco[],int numCroco,Entity proiettile[],Entity granata[], int pipefd, int pipeEvent);