#include <stdio.h>
#include <ncurses/ncurses.h>

int main(void)
{
    initscr();
    raw();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);
    refresh();
    
    return 0;
}