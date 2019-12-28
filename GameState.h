#ifndef ALITA_GAME_STATE_H
#define ALITA_GAME_STATE_H

#include <string>
#include <vector>
#include "GameObject.h"

using GameStateType = std::string;
using GameStateID = std::string;
using GameObjectList = std::vector<GameObject*>;

class GameState{
public:
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

    void addGameObject(GameObject *pGameObj){
        m_gameObjects.push_back(pGameObj);
    };

protected:
    GameObjectList m_gameObjects;
    GameStateID m_gameStateID;

    XMLElement *m_pGameStateInfo = nullptr;
};

using GameStateCreator = GameState*(*)();
#endif