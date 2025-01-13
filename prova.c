#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>
#include <string.h>
#include <fcntl.h>


typedef struct Entity {
    int x, y;
    char sprite;
    int id;
    int event;
    void (*move)(struct Entity *self, int x, int y);
} Entity;

typedef struct Crocodile {
    Entity base;
    int direction;
}Crocodile;

typedef struct Frog {
    Entity base;
    int lives;  
}Frog;
void move(Entity *self, int x, int y) {
    self->x += x;
    self->y += y;
}

void initFrog(){
    Frog frog;
    frog.base.x = 0;
    frog.base.y = 0;
    frog.base.sprite;
    frog.base.id = 1;
    frog.base.move = move;
    frog.lives = 3;
}
void initCrocodile(){
    Crocodile crocodile;
    crocodile.base.x = 0;
    crocodile.base.y = 0;
    crocodile.base.sprite;
    crocodile.base.id = 2;
    crocodile.base.move = move;
    crocodile.direction = 1;
}

void creaPipe(int pipe_fd[2]) {
    if (pipe(pipe_fd) == -1) {
        perror("Errore nella creazione della pipe");
        exit(1);
    }
}

void creaRano(int pipefd[2], pid_t *pid_rana) {
    *pid_rana = fork();
    if (*pid_rana == -1) {
        perror("Errore nella creazione della prima fork");
        _exit(1);
    }

    if (*pid_rana == 0) {  // Processo figlio (rana)
        close(pipefd[0]);  // Chiudi il lato di lettura della pipe
        // Esegui altre operazioni nel processo figlio 'rana'
        rano(pipefd[1]);  // Esempio di scrittura nella pipe
        close(pipefd[1]);  // Chiudi il lato di scrittura della pipe
        _exit(0);  // Esci dal processo figlio
    }
}

void creaCroco(int numCroco,int pipefd[2], pid_t *pid_croco) {
    
    for(int i = 0; i <numCroco; i++) {
        if ((pid_croco[i] = fork()) == 0) {
            close(pipefd[0]); // Chiudi il lato di lettura per i figli
            cocco(i,pipefd[1],i+1);
            _exit(0);
        }
    }
}



void rano(int pipe_fd){
	initFrog();
	while(1) {

	int input = getch();
	//if(input == 'q'){
	//endwin();
	//exit(0);
	//}
	//ho dubbi su questa implementazione,non funziona ma penso sia dovuto al fatto che la pipe sia condivisa.

	switch (input) {
		case KEY_UP:
		rana.x = (rana.x > 1) ? rana.x -1: rana.x;
		break;
		case KEY_DOWN:
		rana.x = (rana.x < LINES-5) ? rana.x +1 : rana.x ;
		break;
		case KEY_LEFT:
		rana.y = (rana.y > 1 ) ? rana.y -1: rana.y;
		break;
		case KEY_RIGHT:
		rana.y = (rana.y < COLS -2 ) ? rana.y +1 : rana.y;
		break;
		}//frecce direzionali per muovere
		write(pipe_fd, &rana, sizeof(MesPos));
	}  
}
void stampRana(int pipefd){
	mvwprintw(gioco,rana.x-1, rana.y, "___");
	mvwprintw(gioco,rana.x, rana.y,   "|_|"); //simbolo usato per rappresentare il personaggio
  	attron(A_REVERSE);
}


void cocco(int nC,int pipefd, int y) {
    initCrocodile();  // Inizia in alto a sinistra
   
    MesPos msg;
    srand(time(NULL) + nC); // Random seed unico per ogni processo
    sleep(rand() % 3 + 1);
    
    while (1) {
       
            if((croco.y) % 2 == 1){
                croco.x++;
            }
            else{    
                croco.x--;
            } 
        
        
        if (croco.x >= COLS) {
            // Scrivi un messaggio di uscita e resetta la posizione
            msg.id = nC;
            msg.event = 1; // Evento: uscito dallo schermo
            write(pipefd, &msg, sizeof(msg));
            croco.x = 0;
            sleep(rand() % 3 + 1); // Pausa randomica
        }

        // Invia la posizione corrente al padre
        msg.id = nC;
        msg.event = croco.x; // Usa "event" per comunicare la posizione
        write(pipefd, &msg, sizeof(msg));

        usleep(200000); // Velocità del movimento
    }
}
void stampCocco(int numCroco,int pipefd,int positions[]) {
	attron(A_REVERSE);
    for (int i = 0; i < numCroco; i++) {
		int row = i / 3 + 1;
		int col;
	
        if (row % 2 == 1) {
		col = (i % 3) * 10;
		} else {
		    col = COLS - (i % 3) * 10 - 10;
			}
     
        croco.x = row;
        croco.y = col + positions[i];
        mvwprintw(gioco, croco.x, croco.y, " [====]=");
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

void funzionamento_gioco (int numCroco,int pipefd,int positions[]) {
	 MesPos msg;
	initFrog();
	initSCroco();
	
    time_t start_time = time(NULL);
	//time_t last_trap_update_time = time(NULL);
	while(1){
		while (1) {
        // Leggi i messaggi dalla pipe
        while (read(pipefd, &msg, sizeof(MesPos)) > 0) {
            if (msg.id == 0) {
                // Messaggio dalla rana
                rana.x = msg.x;
                rana.y = msg.y;
            } else {    
            positions[msg.id] = msg.event;
            }
        }
		//questo controllo if  else if serve per capire chi sta usando il buffer

		stampCocco(numCroco,pipefd,positions);
		stampRana(pipefd);
		wrefresh(gioco);

			if (rana.x == croco.x && rana.y == croco.y){
					mvprintw(LINES / 2, COLS / 2 -5, "preso");
					refresh();
					usleep(DELAYCLOSED);
					endwin();
					exit(0);
				} //win condition
			}
		}
	}

int main(){
    int numCroco = 24;
	pid_t pid_rana;
    pid_t pid_croco[numCroco];
    int pipefd[2];
	initscr();
	noecho();
	curs_set(FALSE);
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE); /*Insieme a setNonBlocking rende le pipe non bloccanti*/
    start_color();
    init_pair(1, COLOR_BLACK, COLOR_GREEN);
    init_pair(2, COLOR_YELLOW, COLOR_BLUE);  
    init_pair(3, COLOR_BLACK, COLOR_YELLOW);
    init_pair(4, COLOR_BLACK, COLOR_GREEN);
    creaPipe(pipefd); 
    setNonBlocking(pipefd[0]);
    
    // Crea il primo e il secondo processo
    creaRano(pipefd, &pid_rana);
    creaCroco(numCroco,pipefd, pid_croco);
    
    close(pipefd[1]); 	
    //int *positions =malloc(numCroco*sizeof(int)); // Posizioni dei coccodrilli

    int positions[numCroco]; // Posizioni dei coccodrilli
    memset(positions, 0, sizeof(positions)); //creazione dinamica 
	funzionamento_gioco(numCroco, pipefd[0],positions); //richiamo la funzione padre 
	
	kill(pid_rana,1);
    for (int i = 0; i < numCroco; i++) {
    kill(pid_croco[i], SIGKILL);  // Uccidi ogni processo
}
	endwin();
	return 0;
}


