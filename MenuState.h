#ifndef ALITA_MENU_STATE_H
#define ALITA_MENU_STATE_H

#include "GameState.h"

class StartMenuState: public GameState {
public:

    virtual void update() override;

    virtual void render() override;

    static const GameStateType s_gameStateType;

    virtual const GameStateType &getGameStateType();

    static GameState *creator();
};

#endif
