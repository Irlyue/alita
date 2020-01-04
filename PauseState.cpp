#include "PauseState.h"
#include "InputHandler.h"
#include "Game.h"


const GameStateType PauseState::s_gameStateType = "PauseState";


const GameStateType &PauseState::getGameStateType(){
    return s_gameStateType;
}

GameState *PauseState::creator(){
    return GCC_NEW PauseState;
}
