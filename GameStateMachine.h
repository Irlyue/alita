#ifndef ALITA_GAME_STATE_MACHINE_H
#define ALITA_GAME_STATE_MACHINE_H

#include <vector>
#include <map>
#include "GameState.h"
#include "MenuState.h"
#include "PlayState.h"
#include "PauseState.h"

class GameStateMachine{
public:
    void update();

    void render();

    bool init(XMLElement *pDoc);

    GameState *currentState();

    void pushState(GameState *pGameState);

    void changeState(GameState *pGameState);

    void popState();

    void destroy();

    GameState* create(const GameStateType &gameStateType);

    void add(const ObjectType &objType, GameStateCreator creator);

private:

    std::vector<GameState*> m_gameStates;

    XMLElement *m_pGameStateInfos = nullptr;

    std::map<GameStateType, GameStateCreator> m_gameStateCreatorMaps;

};

#endif