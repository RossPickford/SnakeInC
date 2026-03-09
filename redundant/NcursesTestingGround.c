#ifdef __MINGW64__
#include <ncurses/ncurses.h>
#endif

#include <stdio.h>

void helloWorld();
void boldChar();
void printBox();
void exampleWindow();
void drawWindow();


int main(void)
{
    // helloWorld();
    // boldChar();
    // printBox();
    // exampleWindow();
    drawWindow();
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
        mvaddch(i,0, ACS_VLINE);
        mvaddch(i,10, ACS_VLINE);
        mvaddch(0,i, ACS_HLINE);
        mvaddch(10,i, ACS_HLINE);
    } 

    refresh();
    getch();
    endwin();
}


WINDOW *create_newwin(int height, int width, int starty, int startx);
void destroy_win(WINDOW *local_win);

void exampleWindow()
{	WINDOW *my_win;
	int startx, starty, width, height;
	int ch;

	initscr();			/* Start curses mode 		*/
	cbreak();			/* Line buffering disabled, Pass on
					 * everty thing to me 		*/
	keypad(stdscr, TRUE);		/* I need that nifty F1 	*/

	height = 3;
	width = 10;
	starty = (LINES - height) / 2;	/* Calculating for a center placement */
	startx = (COLS - width) / 2;	/* of the window		*/
	printw("Press F1 to exit");
	refresh();
	my_win = create_newwin(height, width, starty, startx);

	while((ch = getch()) != KEY_F(1))
	{	switch(ch)
		{	case KEY_LEFT:
				destroy_win(my_win);
				my_win = create_newwin(height, width, starty,--startx);
				break;
			case KEY_RIGHT:
				destroy_win(my_win);
				my_win = create_newwin(height, width, starty,++startx);
				break;
			case KEY_UP:
				destroy_win(my_win);
				my_win = create_newwin(height, width, --starty,startx);
				break;
			case KEY_DOWN:
				destroy_win(my_win);
				my_win = create_newwin(height, width, ++starty,startx);
				break;	
		}
	}
		
	endwin();			/* End curses mode		  */
}

WINDOW *create_newwin(int height, int width, int starty, int startx)
{	WINDOW *local_win;

	local_win = newwin(height, width, starty, startx);
	box(local_win, 0 , 0);		/* 0, 0 gives default characters 
					 * for the vertical and horizontal
					 * lines			*/
	wrefresh(local_win);		/* Show that box 		*/

	return local_win;
}

void destroy_win(WINDOW *local_win)
{	
	/* box(local_win, ' ', ' '); : This won't produce the desired
	 * result of erasing the window. It will leave it's four corners 
	 * and so an ugly remnant of window. 
	 */
	wborder(local_win, ' ', ' ', ' ',' ',' ',' ',' ',' ');
	/* The parameters taken are 
	 * 1. win: the window on which to operate
	 * 2. ls: character to be used for the left side of the window 
	 * 3. rs: character to be used for the right side of the window 
	 * 4. ts: character to be used for the top side of the window 
	 * 5. bs: character to be used for the bottom side of the window 
	 * 6. tl: character to be used for the top left corner of the window 
	 * 7. tr: character to be used for the top right corner of the window 
	 * 8. bl: character to be used for the bottom left corner of the window 
	 * 9. br: character to be used for the bottom right corner of the window
	 */
	wrefresh(local_win);
	delwin(local_win);
}

void drawWindow()
{
    initscr();
    cbreak();
    keypad(stdscr, TRUE);
    refresh();
    WINDOW *myWin;

    myWin = newwin(LINES, COLS, 0, 0);

    box(myWin, 0, 0);

    wrefresh(myWin);

    getch();

    delwin(myWin);

    endwin();
}