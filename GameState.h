#ifndef ALITA_GAME_STATE_H
#define ALITA_GAME_STATE_H

#include <string>
#include <map>
#include "GameObject.h"

using GameStateType = std::string;
using GameStateID = std::string;
using GameObjectMap = std::map<GameObjectID, GameObject*>;

class GameState{
public:
	virtual ~GameState(){}
    virtual bool init(XMLElement *doc);

    virtual void update();
    virtual void render();

    virtual bool onEnter();

    virtual bool onExit();
    virtual void resume() {};

    virtual const GameStateID &getGameStateID(){
        return m_gameStateID;
    }

    virtual const GameStateType &getGameStateType() = 0;

	bool removeGameObject(GameObjectID objID);

    bool addGameObject(GameObject *pGameObj);

protected:
    GameObjectMap m_gameObjects;
    GameStateID m_gameStateID;

    XMLElement *m_pGameStateInfo = nullptr;
};

using GameStateCreator = GameState*(*)();
#endif