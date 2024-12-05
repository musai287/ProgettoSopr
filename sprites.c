#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
void froge();
void croco();

int main() {
    initscr();
    start_color();
    cbreak();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);
    init_pair(1, COLOR_RED, COLOR_BLACK);
    froge();
    croco();
    endwin();   // Clean up ncurses
    return 0;
}




void froge(){
    char frog[2][3] = {
        "___",
        "|_|"
        };

    // Calculate the starting positions to center the frog
    int start_row = (LINES / 2) - 1;  // Centers it more accurately
    int start_col = (COLS / 2) - 2;  // Adjusted to center the frog
    int ch;
    while (1) {
    clear();  // Clear the screen before drawing the frog
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 3; j++) {
            if (frog[i][j] != ' ') {  // Only print non-space characters
                attron(COLOR_PAIR(1));
                mvprintw(start_row + i, start_col + j, "%c", frog[i][j]);
                attroff(COLOR_PAIR(1));
            }
        } 
    }

    refresh();  // Ensure the changes are displayed
    ch = getch(); // Ottieni l'input dell'utente
        if (ch == 'q') break; // Esci se l'utente preme 'q'

        // Aggiorna la posizione in base al tasto premuto
        switch (ch) {
            case KEY_UP:
                if (start_row > 0) start_row--;
                break;
            case KEY_DOWN:
                if (start_row < LINES - 3) start_row++;
                break;
            case KEY_LEFT:
                if (start_col > 0) start_col--;
                break;
            case KEY_RIGHT:
                if (start_col < COLS - 3) start_col++;
                break;
        }
      } // Wait for user input before exiting
    
}



void croco(){
    char crocs[4][6] = {
        "______",
        "|    |",
        "|____|",
        "|____|"
        };

    // Calculate the starting positions to center the frog
    int start_row2 = (LINES / 2) - 8;  // Centers it more accurately
    int start_col2 = (COLS / 2) - 16;  // Adjusted to center the frog
    int ch;
    while (1) {
    clear();  // Clear the screen before drawing the frog
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 6; j++) {
            if (crocs[i][j] != ' ') {  // Only print non-space characters
                attron(COLOR_PAIR(1));
                mvprintw(start_row2 + i, start_col2 + j, "%c", crocs[i][j]);
                attroff(COLOR_PAIR(1));
            }
        } 
    }

    refresh();  // Ensure the changes are displayed
        }
} // Wait for user input before exiting
    
