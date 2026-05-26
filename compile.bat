gcc SnakeGame.c -o game.exe -I "./external/SDL/include" -I "./external/SDL_ttf/include" -L "external/SDL_ttf/lib" -L "./external/SDL/lib" -lSDL3_ttf -lSDL3

gcc -c ui_editor.c -o ui_editor.o -I "./external/SDL/include" -I "./external/SDL_ttf/include"
gcc -c editor_main.c -o editor_main.o -I "./external/SDL/include" -I "./external/SDL_ttf/include"
gcc ui_editor.o editor_main.o -o editor.exe -L "external/SDL_ttf/lib" -L "./external/SDL/lib" -lSDL3_ttf -lSDL3

gcc -c ui_editor.c -o ui_editor.o -I "./external/SDL/include" -I "./external/SDL_ttf/include"
gcc -c test_editor_main.c -o test_editor_main.o -I "./external/SDL/include" -I "./external/SDL_ttf/include"
gcc -c arena.c -o arena.o -I "./external/SDL/include"
gcc -c mainMenuLayout.c -o mainMenuLayout.o -I "./external/SDL/include" -I "./external/SDL_ttf/include"
gcc  arena.o mainMenuLayout.o ui_editor.o test_editor_main.o -o test_editor.exe -L "external/SDL_ttf/lib" -L "./external/SDL/lib" -lSDL3_ttf -lSDL3

gcc editor_ui.c -o editor_ui.exe -I "./external/SDL/include" -I "./external/SDL_ttf/include" -L "external/SDL_ttf/lib" -L "./external/SDL/lib" -lSDL3_ttf -lSDL3

gcc -c quickTestsInC.c -o qtc.o -I "./external/SDL/include" -I "./external/SDL_ttf/include"
gcc  arena.o qtc.o -o arenaTest.exe  -L "external/SDL_ttf/lib" -L "./external/SDL/lib" -lSDL3_ttf -lSDL3