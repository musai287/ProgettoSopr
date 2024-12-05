#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

// Dichiarazione delle funzioni
void draw_sprite(char sprite[][6], int rows, int cols, int *start_row, int *start_col, int color_pair);

int main() {
    initscr();
    start_color();
    cbreak();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);
    init_pair(1, COLOR_GREEN, COLOR_BLACK); // Colore per la rana
    init_pair(2, COLOR_RED, COLOR_BLACK);   // Colore per il coccodrillo

    // Sprite della rana
    char frog[2][4] = {
        "___",
        "|_|"
    };

    // Sprite del coccodrillo
    char crocs[4][7] = {
        "______",
        "|    |",
        "|____|",
        "|____|"
    };

    // Posizioni iniziali
    int frog_row = (LINES / 2) - 1, frog_col = (COLS / 2) - 2;
    int croc_row = (LINES / 2) - 8, croc_col = (COLS / 2) - 16;

    int ch;
    while (1) {
        clear(); // Pulisci lo schermo per ogni frame

        // Disegna la rana
        draw_sprite(frog, 2, 3, &frog_row, &frog_col, 1);

        // Disegna il coccodrillo
        draw_sprite(crocs, 4, 6, &croc_row, &croc_col, 2);

        refresh(); // Aggiorna il terminale

        // Ottieni l'input dell'utente
        ch = getch();
        if (ch == 'q') break; // Esci se l'utente preme 'q'

        // Movimento della rana (tasti direzionali)
        switch (ch) {
            case KEY_UP:
                if (frog_row > 0) frog_row--;
                break;
            case KEY_DOWN:
                if (frog_row < LINES - 2) frog_row++;
                break;
            case KEY_LEFT:
                if (frog_col > 0) frog_col--;
                break;
            case KEY_RIGHT:
                if (frog_col < COLS - 3) frog_col++;
                break;
        }
    }

    endwin(); // Ripristina il terminale
    return 0;
}

// Funzione per disegnare uno sprite
void draw_sprite(char sprite[][6], int rows, int cols, int *start_row, int *start_col, int color_pair) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (sprite[i][j] != ' ') { // Stampa solo caratteri non vuoti
                attron(COLOR_PAIR(color_pair));
                mvprintw(*start_row + i, *start_col + j, "%c", sprite[i][j]);
                attroff(COLOR_PAIR(color_pair));
            }
        }
    }
}
