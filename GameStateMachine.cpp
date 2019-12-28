#include "GameStateMachine.h"

void GameStateMachine::changeState(GameState *pGameState){
    if(!m_gameStates.empty()){
        if(m_gameStates.back()->getGameStateType() == pGameState->getGameStateType())
            return;

        m_gameStates.back()->onExit();
        m_gameStates.pop_back();
    }

    m_gameStates.push_back(pGameState);
    m_gameStates.back()->onEnter();
}

void GameStateMachine::pushState(GameState *pGameState){
    m_gameStates.push_back(pGameState);
    m_gameStates.back()->onEnter();
}

void GameStateMachine::popState(){
    if(!m_gameStates.empty()){
        m_gameStates.back()->onExit();
        m_gameStates.pop_back();
    }
    if(!m_gameStates.empty()){
        m_gameStates.back()->resume();
    }
}

void GameStateMachine::update(){
    if(!m_gameStates.empty()){
        m_gameStates.back()->update();
    }
}

void GameStateMachine::render(){
    if(!m_gameStates.empty()){
        m_gameStates.back()->render();
    }
}

bool GameStateMachine::init(XMLElement *pDoc){
    m_pGameStateInfos = pDoc;
    return true;
}

void GameStateMachine::add(const ObjectType &objType, GameStateCreator creator){
    m_gameStateCreatorMaps[objType] = creator;
}

GameState *GameStateMachine::create(const GameStateType &gsType){
    auto it = m_gameStateCreatorMaps.find(gsType);
    if(it != m_gameStateCreatorMaps.cend()){
        GameState *pState =  it->second();
        auto p = m_pGameStateInfos->FirstChildElement(gsType.c_str());
        pState->init(p);

        return pState;
    }
    printf("No such game state as `%s`\n", gsType.c_str());
    return nullptr;
}

void GameStateMachine::destroy(){
    while(!m_gameStates.empty()){
        m_gameStates.back()->onExit();
        m_gameStates.pop_back();
    }
}

GameState *GameStateMachine::currentState(){
    if(!m_gameStates.empty()){
        return m_gameStates.back();
    }
    return nullptr;
}