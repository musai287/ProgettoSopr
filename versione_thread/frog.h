#pragma once
#include <ncurses.h>
#include "struct.h"

// Thread per la rana
void *threadRana(void *arg);

// Thread per una singola granata
void *threadGranata(void *arg);

// Funzione di input
int movimento();
