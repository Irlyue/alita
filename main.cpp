#include "Game.h"
#include "PackedImages.h"
#include "GameMap.h"
#include <locale>
#include <codecvt>
#include <cwchar>
#include <iostream>

int main(int, char**){
    // disable output buffer, so the outputs are flushed immediately.
    setbuf(stdout, nullptr);

    Game *game = Game::getInstance();
    if(!game->init("Alita", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480)){
        printf("Woops! Something is wrong!\n");
        return -1;
    }

    printf("Initilization success!\n");

    Uint32 tic, elapsed;
    int frame = 0;

    while(game->isRunning()){
        tic = SDL_GetTicks();
        frame++;
        game->handleEvents();
        game->update();
        game->render();

        elapsed = SDL_GetTicks() - tic;
        if(elapsed < FRAME_TIME){
            SDL_Delay(FRAME_TIME - elapsed);
        }
        if(frame % 100 == 0)
            printf("%d\n", elapsed);
    }

    game->destroy();

    printf("Done!\n");
    return 0;
}