#include <windows.h>
#include <stdio.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow)
{
    if (GetConsoleWindow() == NULL)
        printf("Console returns null\n");

    ShowWindow(GetConsoleWindow(), nCmdShow);
    ShowWindow(GetConsoleWindow(), SW_SHOWMAXIMIZED);
    // MessageBox(NULL, TEXT ("Testing"), TEXT ("HelloMsg"), 0);
    getchar();

    return 0;
}