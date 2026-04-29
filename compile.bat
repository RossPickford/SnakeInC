gcc SnakeGame.c -o game.exe -I "./external/SDL/include" -I "./external/SDL_ttf/include" -L "external/SDL_ttf/lib" -L "./external/SDL/lib" -lSDL3_ttf -lSDL3

gcc -c ui_editor.c -o ui_editor.o -I "./external/SDL/include" -I "./external/SDL_ttf/include"
gcc -c editor_main.c -o editor_main.o -I "./external/SDL/include" -I "./external/SDL_ttf/include"
gcc ui_editor.o editor_main.o -o editor.exe -L "external/SDL_ttf/lib" -L "./external/SDL/lib" -lSDL3_ttf -lSDL3

gcc editor_ui.c -o editor_ui.exe -I "./external/SDL/include" -I "./external/SDL_ttf/include" -L "external/SDL_ttf/lib" -L "./external/SDL/lib" -lSDL3_ttf -lSDL3


