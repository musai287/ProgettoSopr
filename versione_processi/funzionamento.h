#pragma once
#include <ncurses.h>
#include "struct.h"

#include "frog.h"
#include "croco.h"



void finestre(Fin *fin1, Fin *fin2);
void gestisci_vite(int vite, time_t start_time);
void funzionamento_gioco (int pipe_fd);