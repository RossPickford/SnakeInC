#ifdef __MINGW64__
    #include <ncurses/ncurses.h>
#endif

#include <stdio.h>

void helloWorld();

int main(void)
{
    helloWorld();
    return 0;
}

void helloWorld()
{
    initscr();                 /* Start curses mode 		  */
    printw("Hello World !!!"); /* Print Hello World		  */
    refresh();                 /* Print it on to the real screen */
    getch();                   /* Wait for user input */
    endwin();                  /* End curses mode		  */
}