gcc SnakeGame.c -o game.exe -I "./external/SDL/include" -I "./external/SDL_ttf/include" -L "external/SDL_ttf/lib" -L "./external/SDL/lib" -lSDL3_ttf -lSDL3

gcc -c ui_editor.c -o ui_editor.o -I "./external/SDL/include" -I "./external/SDL_ttf/include"
gcc -c arena.c -o arena.o
gcc -c test_editor_main.c -o test_editor_main.o -I "./external/SDL/include" -I "./external/SDL_ttf/include"
gcc arena.o ui_editor.o editor_main.o -o editor2.exe -L "external/SDL_ttf/lib" -L "./external/SDL/lib" -lSDL3_ttf -lSDL3

gcc -c ui_editor.c -o ui_editor.o -I "./external/SDL/include" -I "./external/SDL_ttf/include"
gcc -c test_editor_main.c -o test_editor_main.o -I "./external/SDL/include" -I "./external/SDL_ttf/include"
gcc -c arena.c -o arena.o -I "./external/SDL/include"
gcc -c mainMenuLayout.c mainMenuLayout.o -I "./external/SDL/include" -I "./external/SDL_ttf/include"
gcc  arena.o mainMenuLayout.o test_editor_main.o -o test_editor.exe -L "external/SDL_ttf/lib" -L "./external/SDL/lib" -lSDL3_ttf -lSDL3

gcc editor_ui.c -o editor_ui.exe -I "./external/SDL/include" -I "./external/SDL_ttf/include" -L "external/SDL_ttf/lib" -L "./external/SDL/lib" -lSDL3_ttf -lSDL3

gcc arena.c -o arena.exe -I "./external/SDL/include" -L "./external/SDL/lib" -lSDL3
