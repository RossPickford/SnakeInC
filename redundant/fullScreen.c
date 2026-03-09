#include <stdio.h>
#include <windows.h>
#include <ncurses/curses.h>
// #include <pdcurses/curses.h>

void drawWindow(int y, int x);

int main()
{
    CONSOLE_SCREEN_BUFFER_INFOEX info = {0};
    int width, height;
    HANDLE hConsole = NULL;

    // maximize window
    // ShowWindow(GetConsoleWindow(), SW_MAXIMIZE);

    STARTUPINFO si = {0};
    PROCESS_INFORMATION pi = {0};

    si.cb = sizeof(si);

    if (!CreateProcess(NULL, "wt -F", NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
    {
        printf("CreateProcess failed (%lu).\n", GetLastError());
        return 1;
    }

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    // print width/height of this window in terms of columns and rows
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    info.cbSize = sizeof(info);
    GetConsoleScreenBufferInfoEx(hConsole, &info);

    width = info.srWindow.Right;
    height = info.srWindow.Bottom;

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
    refresh();
    
    WINDOW *myWin;
    
    myWin = newwin(y, x, 0, 0);
    
    box(myWin, 0, 0);
    
    mvprintw(y / 2, x / 2, "Size of this window is %d x %d", x, y);
    wrefresh(myWin);

    getch();

    delwin(myWin);

    endwin();
}