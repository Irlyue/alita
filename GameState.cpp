#include "GameState.h"
#include "GameObjectFactory.h"


bool GameState::init(XMLElement *doc){
    // simply store the pointer for later usage
    m_pGameStateInfo = doc;
    return true;
};

void GameState::update(){
    for(auto &obj: m_gameObjects){
        obj->update();
    }
}

void GameState::render(){
    for(auto &obj: m_gameObjects){
        obj->draw();
    }
}

bool GameState::onEnter() {
    printf("Enter %s...\n", getGameStateType().c_str());

    auto pGameObjectFactory = GameObjectFactory::getInstance();
    bool success = true;
    auto it = m_pGameStateInfo->FirstChild();
    for(; it; it = it->NextSibling()){
        printf("%s\n", it->Value());
        GameObject *pGameObj = pGameObjectFactory->create(it->Value());
        addGameObject(pGameObj);
    }
    return success;
}


bool GameState::onExit() {
    printf("Exit %s...\n", getGameStateType().c_str());
    for(auto &obj: m_gameObjects){
        obj->destroy();
    }
    m_gameObjects.clear();
    return true;
};