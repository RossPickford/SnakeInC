#ifdef __MINGW64__
#include <ncurses/ncurses.h>
#endif

#include <stdio.h>

void helloWorld();
void boldChar();
void printBox();

int main(void)
{
    // helloWorld();
    // boldChar();
    printBox();
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

void boldChar()
{
    int ch;

    initscr();            /* Start curses mode 		*/
    raw();                /* Line buffering disabled	*/
    keypad(stdscr, TRUE); /* We get F1, F2 etc..		*/
    noecho();             /* Don't echo() while we do getch */

    printw("Type any character to see it in bold\n");
    ch = getch();                 /* If raw() hadn't been called
                                   * we have to press enter before it
                                   * gets to the program 		*/
    if (ch == KEY_F(1))           /* Without keypad enabled this will */
        printw("F1 Key pressed"); /*  not get to us either	*/
                                  /* Without noecho() some ugly escape
                                   * charachters might have been printed
                                   * on screen			*/
    else
    {
        printw("The pressed key is ");
        attron(A_BOLD);
        printw("%c", ch);
        attroff(A_BOLD);
    }
    refresh(); /* Print it on to the real screen */
    getch();   /* Wait for user input */
    endwin();  /* End curses mode		  */
}

void printBox()
{
    initscr();
    raw();

    mvaddch(0,0, ACS_ULCORNER);
    mvaddch(10,0, ACS_LLCORNER);
    mvaddch(0,10, ACS_URCORNER);
    mvaddch(10,10, ACS_LRCORNER);

    for (int i = 1; i < 10; i++)
    {
        mvaddch(0,i, ACS_HLINE);
        mvaddch(i,0, ACS_VLINE);
        mvaddch(i,10, ACS_VLINE);
        mvaddch(10,i, ACS_HLINE);
    }

    refresh();
    getch();
    endwin();
}