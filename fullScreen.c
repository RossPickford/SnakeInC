#include <stdio.h>
#include <windows.h>
#include <ncurses/curses.h>

void drawWindow(int y, int x);

int main()
{
    CONSOLE_SCREEN_BUFFER_INFOEX info = {0};
    int width, height;
    HANDLE hConsole = NULL;

    // maximize window
    ShowWindow(GetConsoleWindow(), SW_SHOWMAXIMIZED);

    // print width/height of this window in terms of columns and rows
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    info.cbSize = sizeof(info);
    GetConsoleScreenBufferInfoEx(hConsole, &info);

    width = info.srWindow.Right + 1;
    height = info.srWindow.Bottom + 1;

    drawWindow(height, width);
    // printf("Size of this window is %d x %d\n", width, height);
    // getchar();

    return 0;
}

void drawWindow(int y, int x)
{
    initscr();
    cbreak();
    keypad(stdscr, TRUE);
    mvprintw(y / 2, x / 2, "Size of this window is %d x %d", x, y);

    refresh();
    WINDOW *myWin;

    myWin = newwin(y, x, 0, 0);

    box(myWin, 0, 0);

    wrefresh(myWin);

    getch();

    delwin(myWin);

    endwin();
}