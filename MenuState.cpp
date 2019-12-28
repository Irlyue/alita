#include "MenuState.h"
#include "Game.h"

const GameStateType StartMenuState::s_gameStateType = "StartMenuState";

const GameStateType &StartMenuState::getGameStateType() {
    return s_gameStateType;
}

GameState *StartMenuState::creator(){
    return new StartMenuState;
}


void StartMenuState::update(){
    auto pStateMachine = g_alita->getStateMachine();
    if(g_alita->getInputHandler()->getBtnState(SDL_BUTTON_LEFT)){
        pStateMachine->changeState(pStateMachine->create("PlayState"));
    }
}

void StartMenuState::render(){
    // GameState::render();

    SDL_SetRenderDrawColor(g_alita->getRenderer(), 0x00, 0x00, 0x00, 0xff);
    SDL_RenderClear(g_alita->getRenderer());
    int width = 400, height = 200;
    int x = (g_alita->getWindowWidth() - width)/2, y = (g_alita->getWindowHeight() - height)/2;
    g_alita->getTextureManager()->drawTile("GAME", 0, x, y, width, height, g_alita->getRenderer());
}