#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>

#include "struct.h"
#include "croco.h"

void cocco(int nC,int pipefd, int y) {
    initSCroco();  // Inizia in alto a sinistra
    int x = 0;
    Message msg;
    srand(time(NULL) + nC); // Random seed unico per ogni processo
    sleep(rand() % 3 + 1);
    while (1) {
        x++;
        if (x >= COLS) {
            // Scrivi un messaggio di uscita e resetta la posizione
            msg.id = nC;
            msg.event = 1; // Evento: uscito dallo schermo
            write(pipefd, &msg, sizeof(msg));
            x = 0;
            sleep(rand() % 3 + 1); // Pausa randomica
        }

        // Invia la posizione corrente al padre
        msg.id = nC;
        msg.event = x; // Usa "event" per comunicare la posizione
        write(pipefd, &msg, sizeof(msg));

        usleep(200000); // Velocità del movimento
    }
}
void stampCocco(int numCroco,int pipefd,int positions[]) {
for (int i = 0; i < numCroco; i++) {
					int row = i / 3 + 1;
					int col;
					if (row % 2 == 1) {
						col = (i % 3) * 10;
					} else {
						col = COLS - (i % 3) * 10 - 10;
					}
					mvwprintw(gioco, row, col + positions[i], " [====]=");
				}
				attron(A_REVERSE);
				}