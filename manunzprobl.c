#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>


int pipe_gioco[2];

typedef struct Fin {
    int height;
    int width;
    int starty;
    int startx;
} Fin;
void finestre(Fin *fin1, Fin *fin2);
void giocatore(int *posizione, int pipe);
void nemicoVert(int *posizione, int pipe);
void nemicoOrizz(int *posizione, int pipe);
void gioco();

WINDOW *winScore, *winGame;
int max_x, max_y;
int max_x2, max_y2;

int main () 
{
    initscr();
    noecho();
    curs_set(0);
    cbreak();
    nodelay(stdscr, true);
    keypad(stdscr, true);
   
    srand(time(NULL) ^ getpid());

   
    Fin fin1 = {3, COLS-1, 3, 0};
    Fin fin2 = {LINES-3, COLS-1, 3, 0};
    finestre(&fin1, &fin2);
    getmaxyx(winGame, max_y, max_x);

    pipe(pipe_gioco);
    pid_t pid_giocatore = fork();
    if (pid_giocatore == 0) {
        close(pipe_gioco[0]);
        int posizione[2] = {(max_x -1) / 2, (max_y -1) / 2};
        while (1) {
            giocatore(posizione, pipe_gioco[1]);
            usleep(80000);
        }
        exit(0);
    }
    pid_t pid_nemicoVert = fork();
    if (pid_nemicoVert == 0) {
        close(pipe_gioco[0]);
        srand(time(NULL) ^ getpid());
        int posizione[2] = {rand() % (max_x -1) + 1, 1};  //prima coordinata x, seconda y
        while (1) {
            nemicoVert(posizione, pipe_gioco[1]);
            usleep(80000);
        }
        exit(0);
    }
    pid_t pid_nemicoOrizz = fork();
    if (pid_nemicoOrizz == 0) {
        close(pipe_gioco[0]);
        srand(time(NULL) ^ getpid());
        int posizione[2] = {2, rand() % (max_y -1) + 1};
        while (1) {
            nemicoOrizz(posizione, pipe_gioco[1]);
            usleep(80000);
        }
        exit(0);
    }
    close(pipe_gioco[1]);
    gioco();
    endwin();
    return 0;


}
void finestre(Fin *fin1, Fin *fin2) {
    refresh();
    winScore = newwin(fin1->height, fin1->width, fin1->starty, fin1->startx);
    winGame  = newwin(fin2->height, fin2->width, fin2->starty, fin2->startx);
    // Imposta il colore di sfondo della finestra gioco
    box(winScore, 0, 0);    // Bordo della finestra vita
    box(winGame, 0, 0); // Bordo della finestra gioco
    wrefresh(winScore);  // Aggiorna la finestra vita
    wrefresh(winGame); // Aggiorna la finestra gioco
}
void giocatore(int *posizione, int pipe)
{
    int ch = getch();
    if (ch == KEY_RIGHT && posizione[0] < max_x - 1) posizione[0]++;
    if (ch == KEY_LEFT && posizione[0] > 1) posizione[0]--;
    if (ch == KEY_UP && posizione[1] > 1) posizione[1]--;
    if (ch == KEY_DOWN && posizione[1] < max_y - 2) posizione[1]++;

    int message[3] = {0, posizione[0], posizione[1]};
    write(pipe, message, sizeof(int) * 3);
    flushinp();

}

void nemicoOrizz(int *posizione, int pipe)
{
    if (posizione [0] < max_x -1) {
        posizione[0]++;
    } else {
        posizione[0] = 1;
        posizione[1] = rand() % (max_y - 1) + 1;
    }
    int message[3] = {1, posizione[0], posizione[1]};
    write(pipe, message, sizeof(int) * 3);

}

void nemicoVert(int *posizione, int pipe)
{
    if (posizione[1] < max_y -1 ) {
        posizione[1]++;
    } else {
        posizione[1] = 1;
        posizione[0] = rand() % (max_x -1) + 1;
    }
    int message[3] = {2, posizione[0], posizione[1]};
    write(pipe, message, sizeof(int) * 3);
}

void gioco()
{
    //troppe variabili che non ho voglia di ripulire
    getmaxyx(winGame, max_y, max_x);

    int giocatore_pos[2] = {max_x / 2, max_y /2};
    int nemicoOrizz_pos[2] = {2, rand() % (max_y -1) + 1}; //prima coordinata x, seconda y
    int nemicoVert_pos[2] = {rand() % (max_x -1) + 1, 1};

    int scudi = 3;
    time_t start_time = time(NULL);

    time_t ballon_time = time(0);
    int ballon_pos[2];
    ballon_pos[0] = rand() % (max_x -1) +1;
    ballon_pos[1] = rand() % (max_y -1) + 1;
    int message[3];
    int toccato = 0;
    time_t toccato_time = time(0);
   

    srand(time(NULL) ^ getpid());


    while (1)
    {   getmaxyx(winGame, max_y, max_x);
        getmaxyx(winScore, max_y2, max_x2);
        read(pipe_gioco[0], message, sizeof(int) * 3);
        if(message[0] == 0){
            giocatore_pos[0] = message[1];
            giocatore_pos[1] = message[2];
        }
        else if(message[0] == 1){

        
            nemicoOrizz_pos[0] = message[1];
            nemicoOrizz_pos[1] = message[2];
        }
         else if(message[0] == 2){
            nemicoVert_pos[0] = message[1];  
            nemicoVert_pos[1] = message[2];
        }
        werase(winGame);
        box(winGame, 0, 0); 
        mvwprintw(winGame, nemicoOrizz_pos[1], nemicoOrizz_pos[0], "+");  
        mvwprintw(winGame, nemicoVert_pos[1], nemicoVert_pos[0], "x");
        mvwprintw(winGame, ballon_pos[1], ballon_pos[0], "o");
        mvwprintw(winGame, giocatore_pos[1], giocatore_pos[0], "#");
        wrefresh(winGame);
        

        werase(winScore);
        box(winScore, 0, 0); 
        mvwprintw(winScore, 1, 1, "Scudi: %d", scudi);
        mvwprintw(winScore, 1, COLS / 2, "Tempo: %d", (int)(time(NULL) - start_time)); 
        wrefresh(winScore);



        time_t current_time = time(NULL);
        if ((current_time - ballon_time) >= 10) {
            ballon_pos[0] = rand() % (max_x -1) +1;
            ballon_pos[1] = rand() % (max_y -1) + 1;
            ballon_time = current_time;
        }
        if (giocatore_pos[0] == ballon_pos[0] && giocatore_pos[1] == ballon_pos[1]) {
                if (scudi <= 6) {
                    scudi++;
                }
                sleep(1);
                ballon_pos[0] = rand() % (max_x -1) +1;
                ballon_pos[1] = rand() % (max_y - 1) + 1;
            }
        if (toccato == 1 && (current_time - toccato_time) >= 4) {
            toccato = 0;
        }
        else if (toccato == 0)
        {
            if (giocatore_pos[0] == nemicoOrizz_pos[0] && giocatore_pos[1] == nemicoOrizz_pos[1] || giocatore_pos[0] == nemicoVert_pos[0] && giocatore_pos[1] == nemicoVert_pos[1]) {
                scudi--;
                toccato = 1;
                toccato_time = time(0);
                giocatore_pos[0] = max_x / 2;
                giocatore_pos[1] = max_y / 2;
                sleep(1);
                if (scudi == 0) 
                {
                    mvwprintw(winGame, max_y / 2, (max_x / 2) - 4, "GAME OVER");
                    mvwprintw(winGame, (max_y / 2) + 1, (max_x / 2) - 6, "Tempo: %d", (int)(time(NULL) - start_time));
                    sleep(2);
                    break;
                }
            }
        }
    }
}  