#pragma once
#include <ncurses.h>
#include "struct.h"

#include "frog.h"
#include "croco.h"
void gestisci_vite(int vite, time_t start_time);
void finestre(Fin *fin1, Fin *fin2);
void funzionamento_gioco (int pipe_fd);