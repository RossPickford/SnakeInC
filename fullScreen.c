// Source - https://stackoverflow.com/a/48244995
// Posted by selbie, modified by community. See post 'Timeline' for change history
// Retrieved 2026-02-19, License - CC BY-SA 3.0
#include <stdio.h>
#include <windows.h>
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

    width = info.srWindow.Right+1;
    height = info.srWindow.Bottom+1;
    printf("Size of this window is %d x %d\n", width, height);
    getchar();

    return 0;
}
