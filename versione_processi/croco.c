#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>

#include "struct.h"
#include "croco.h"

void cocco(int pipe_fd) {
    initSCroco();  // Inizia in alto a sinistra
    while (1) {
        usleep(DELAY);
        
        // Simuliamo un movimento verticale
        int dx = 2;

        // Calcolo del nuovo valore di y con effetto Pacman
        croco.y = (croco.y + dx) % COLS;

        // Se y diventa negativa (es. quando dy è negativo), riportiamola sul lato opposto
        if (croco.y < 0) {
            croco.y += COLS;
        }

        write(pipe_fd, &croco, sizeof(MesPos));
    }
}