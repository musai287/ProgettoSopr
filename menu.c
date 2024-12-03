#include <ncurses.h>
#include <stdlib.h>

int main(){
    initscr();
    noecho();
    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLUE);
    curs_set(FALSE);
    bkgd(COLOR_PAIR(1));
    keypad(stdscr, TRUE);
    box(stdscr, 0, 0);
    wrefresh(stdscr);
    mvprintw( 2,5, "Press any key to exit");
    wgetch(stdscr);
    endwin();
    exit(0);
    return 0;

}