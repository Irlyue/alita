#include "Game.h"
#include "PackedImages.h"
#include "GameMap.h"
#include <locale>
#include <codecvt>
#include <cwchar>
#include <iostream>
#include <cstdio>

int main(int, char**){
    // disable output buffer, so the outputs are flushed immediately.
    setbuf(stdout, nullptr);
    setbuf(stderr, nullptr);

#if defined WIN32
    int tmpDbgFlag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
    tmpDbgFlag |= _CRTDBG_LEAK_CHECK_DF;
    _CrtSetDbgFlag(tmpDbgFlag);
#endif

    //_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
    //_CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDOUT);

    Game *game = Game::getInstance();
    if(!game->init("Alita", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 730, 480)){
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
        if(frame % 10 == 0)
            printf("%d\n", elapsed);
    }

    game->destroy();
    delete game;

    printf("Done!\n");

#if defined WIN32
    _CrtDumpMemoryLeaks();
#endif
    return 0;
}