#pragma once
#include <ncurses.h>
#include "struct.h"


// Rimuoviamo processi, pipe, etc.
// Definiamo le funzioni thread

void *threadRana(void *arg);       // Sostituisce processoRana()
void *threadGranata(void *arg);    // Sostituisce processoGranata()

// Rimane la funzione input, se vuoi
int movimento();
