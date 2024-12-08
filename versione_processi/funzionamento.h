#pragma once
#include <ncurses.h>
#include "struct.h"
#include <string.h>
#include <fcntl.h>

#include "frog.h"
#include "croco.h"


void setNonBlocking(int pipe_fd);
void finestre(Fin *fin1, Fin *fin2);
void gestisci_vite(int vite, time_t start_time);
void funzionamento_gioco (int numCroco, int pipeRana, int pipeCroco, int positions[]);