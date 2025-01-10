#pragma once

#include <ncurses.h>

#define DELAY 1000000 //0.001
#define DELAYCLOSED 2000000 //2 secondi

void rano(int pipe_fd);
void stampRana(int pipefd);