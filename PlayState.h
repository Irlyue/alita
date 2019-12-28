#ifndef ALITA_PLAY_STATE_H
#define ALITA_PLAY_STATE_H

#include "GameState.h"
#include "Level.h"

class PlayState: public GameState{
public:

    virtual bool init(XMLElement *doc) override;

    virtual void render() override;

    const static GameStateType s_gameStateType;

    static GameState *creator();

    virtual const GameStateType &getGameStateType();

    virtual void update() override;

private:
    Level *m_pLevel = nullptr;

};

#endif