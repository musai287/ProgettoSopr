#pragma once
#include <ncurses.h>
#include <string.h>
#include <fcntl.h>

#include "struct.h"
#include "frog.h"
#include "croco.h"
#include "collisioni.h"

void finestre();
void gestisci_vite(int vite, time_t start_time);
void funzionamento_gioco(SharedData *sd);