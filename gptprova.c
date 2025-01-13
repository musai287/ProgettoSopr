#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>
#include <string.h>
#include <fcntl.h>

// Definizione delle strutture
typedef struct Entity {
    int x, y;
    char sprite;
    int id;
    int event;
    void (*entity_move)(struct Entity *self, int x, int y);
} Entity;

typedef struct Crocodile {
    Entity base;
    int direction;
} Crocodile;

typedef struct Frog {
    Entity base;
    int lives;  
} Frog;

// Funzione per muovere un'entità
void entity_move(Entity *self, int x, int y) {
    self->x += x;
    self->y += y;
}

// Inizializzazione della rana
Frog initFrog() {
    Frog frog;
    frog.base.x = 5;
    frog.base.y = 5;
    frog.base.sprite = 'R';
    frog.base.id = 1;
    frog.base.entity_move = entity_move;
    frog.lives = 3;
    return frog;
}

// Inizializzazione del coccodrillo
Crocodile initCrocodile() {
    Crocodile crocodile;
    crocodile.base.x = 0;
    crocodile.base.y = 0;
    crocodile.base.sprite = 'C';
    crocodile.base.id = 2;
    crocodile.base.entity_move = entity_move;
    crocodile.direction = 1;
    return crocodile;
}

void creaPipe(int pipe_fd[2]) {
    if (pipe(pipe_fd) == -1) {
        perror("Errore nella creazione della pipe");
        exit(1);
    }
}

void frog_process(int pipe_fd) {
    Frog frog = initFrog();
    while (1) {
        int input = getch();
        switch (input) {
            case KEY_UP:
                frog.base.entity_move(&frog.base, -1, 0);
                break;
            case KEY_DOWN:
                frog.base.entity_move(&frog.base, 1, 0);
                break;
            case KEY_LEFT:
                frog.base.entity_move(&frog.base, 0, -1);
                break;
            case KEY_RIGHT:
                frog.base.entity_move(&frog.base, 0, 1);
                break;
        }
        write(pipe_fd, &frog.base, sizeof(Entity));
    }
}

void crocodile_process(int pipe_fd, int direction) {
    Crocodile croc = initCrocodile();
    croc.direction = direction;
    while (1) {
        croc.base.entity_move(&croc.base, 0, croc.direction);
        if (croc.base.y >= COLS || croc.base.y < 0) {
            croc.direction = -croc.direction;
        }
        write(pipe_fd, &croc.base, sizeof(Entity));
        usleep(200000);
    }
}

void setNonBlocking(int pipefd) {
    int flags = fcntl(pipefd, F_GETFL, 0);
    if (flags == -1) {
        perror("fcntl F_GETFL");
        exit(EXIT_FAILURE);
    }

    if (fcntl(pipefd, F_SETFL, flags | O_NONBLOCK) == -1) {
        perror("fcntl F_SETFL");
        exit(EXIT_FAILURE);
    }
}

int main() {
    int pipe_fd[2];
    creaPipe(pipe_fd);
    setNonBlocking(pipe_fd[0]);

    initscr();
    noecho();
    curs_set(FALSE);
    keypad(stdscr, TRUE);

    pid_t pid_frog = fork();
    if (pid_frog == 0) {
        close(pipe_fd[0]);
        frog_process(pipe_fd[1]);
        exit(0);
    }

    pid_t pid_croc = fork();
    if (pid_croc == 0) {
        close(pipe_fd[0]);
        crocodile_process(pipe_fd[1], 1);
        exit(0);
    }

    close(pipe_fd[1]);

    Entity entity;
    while (1) {
        while (read(pipe_fd[0], &entity, sizeof(Entity)) > 0) {
            mvprintw(entity.x, entity.y, "%c", entity.sprite);
        refresh();
        }
        
        usleep(50000);
    }

    endwin();
    return 0;
}
