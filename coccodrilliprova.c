#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <time.h>

struct Message {
    int id;
    int event;
};
void crocodile_process(int nC, int pipe_fd, int y) {
    int x = 0;
    struct Message msg;

    srand(time(NULL) + nC); // Random seed unico per ogni processo

    while (1) {
        x++;
        if (x >= COLS) {
            // Scrivi un messaggio di uscita e resetta la posizione
            msg.id = nC;
            msg.event = 1; // Evento: uscito dallo schermo
            write(pipe_fd, &msg, sizeof(msg));
            x = 0;
            sleep(rand() % 3 + 1); // Pausa randomica
        }

        // Invia la posizione corrente al padre
        msg.id = nC;
        msg.event = x; // Usa "event" per comunicare la posizione
        write(pipe_fd, &msg, sizeof(msg));

        usleep(200000); // Velocità del movimento
    }
}

void padre (int num_crocodiles, int pipe_fd, int positions[]) {
    struct Message msg;
    while (1) {
        // Leggi dalla pipe
        if (read(pipe_fd, &msg, sizeof(msg)) > 0) {
            if (msg.event == 1) {
                // Evento: uscito dallo schermo (gestione facoltativa)
            } else {
                // Aggiorna posizione
                positions[msg.id] = msg.event;
            }

            // Disegna lo schermo
            clear();
            for (int i = 0; i < num_crocodiles; i++) {
                mvprintw(i + 1, positions[i], " [====]");
            }
            refresh();
        }
    }

}
int main() {
    initscr();
    noecho();
    curs_set(0);

    int num_crocodiles = 20;
    pid_t pids[num_crocodiles];
    int pipe_fd[2];

    if (pipe(pipe_fd) == -1) {
        perror("Errore nella creazione della pipe");
        exit(1);
    }

    int positions[num_crocodiles]; // Posizioni dei coccodrilli
    memset(positions, 0, sizeof(positions));

    // Creazione dei processi figli
    for (int i = 0; i < num_crocodiles; i++) {
        if ((pids[i] = fork()) == 0) {
            close(pipe_fd[0]); // Chiudi il lato di lettura per i figli
            crocodile_process(i, pipe_fd[1], i + 1);
            exit(0);
        }
    }

    close(pipe_fd[1]); // Chiudi il lato di scrittura nel processo padre
 //potrebbe stare nel main fino a qui 
    padre(num_crocodiles, pipe_fd[0], positions);
    
    endwin();
    return 0;
}

/*
void crocodile_process(int nC, int pipe_fd, int y) {
    int x = 0; // Ogni coccodrillo ha una riga diversa
    
    
    struct Message msg;

    srand(time(NULL) + nC); // Random seed unico per ogni processo

    while (1) {
        // Movimento orizzontale
        x++;
        if (x >= COLS) {
            // Scrivi un messaggio nella pipe
            msg.id = nC;
            msg.event = 1; // Evento: uscito dallo schermo
            write(pipe_fd, &msg, sizeof(msg));

            // Reset posizione e pausa randomica
            x = 0;
            sleep(rand() % 3 + 1);

            // Notifica che il coccodrillo è stato ricreato
            msg.event = 2; // Evento: ricreato
            write(pipe_fd, &msg, sizeof(msg));
        }
        clear();
        // Aggiorna il display
        mvprintw(y, x, " [====]");
        refresh();
        
        usleep(200000); // Velocità del movimento
    }
}

int main() {
    initscr();
    noecho();
    curs_set(0);

    int y = 0;
    int num_crocodiles = 5;
    pid_t pids[num_crocodiles];
    int pipe_fd[2];

    // Creazione della pipe
    if (pipe(pipe_fd) == -1) {
        perror("Errore nella creazione della pipe");
        exit(1);
    }

    // Creazione dei processi figli
    for (int i = 0; i < num_crocodiles; i++) {
        y++;
        if ((pids[i] = fork()) == 0) {
            crocodile_process(i, pipe_fd[1],y);
            close(pipe_fd[0]); // Chiudi il lato di lettura nella pipe
            exit(0);
        }
    }

    close(pipe_fd[1]); // Chiudi il lato di scrittura della pipe nel processo padre

    // Lettura dei messaggi nella pipe
    struct Message msg;
    
    // Aspetta la terminazione (mai in questo caso)
    for (int i = 0; i < num_crocodiles; i++) {
        wait(NULL);
    }

    endwin();
    return 0;
}
*/