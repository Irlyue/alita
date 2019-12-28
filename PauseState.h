#ifndef ALITA_PAUSE_H
#define ALITA_PAUSE_H

#include "GameState.h"
class PauseState: public GameState{
public:
    static GameState *creator();

    static const GameStateType s_gameStateType;

    virtual const GameStateType &getGameStateType();

};


#endif