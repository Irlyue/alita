#ifndef ALITA_GAME_STATE_MACHINE_H
#define ALITA_GAME_STATE_MACHINE_H

#include <vector>
#include <map>
#include <memory>
#include "GameState.h"
#include "MenuState.h"
#include "PlayState.h"
#include "PauseState.h"

using GameStatePtr = std::shared_ptr<GameState>;

class GameStateMachine{
public:
	~GameStateMachine(){destroy();}

    void update();

    void render();

    bool init(XMLElement *pDoc);

	GameStatePtr currentState();

    void pushState(GameStatePtr pGameState);

    void changeState(GameStatePtr pGameState);

    void popState();

    void destroy();

	GameStatePtr create(const GameStateType &gameStateType);

    void add(const ObjectType &objType, GameStateCreator creator);

private:

    std::vector<GameStatePtr> m_gameStates;

    XMLElement *m_pGameStateInfos = nullptr;

    std::map<GameStateType, GameStateCreator> m_gameStateCreatorMaps;

};

#endif