#include "GameState.h"
#include "GameObjectFactory.h"


bool GameState::init(XMLElement *doc){
    // simply store the pointer for later usage
    m_pGameStateInfo = doc;
    return true;
};

void GameState::update(){
    for(auto it: m_gameObjects){
        it.second->update();
    }
}

void GameState::render(){
    for(auto it: m_gameObjects){
        it.second->draw();
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
    for(auto it: m_gameObjects){
        delete it.second;
    }

    m_gameObjects.clear();
    return true;
};

bool GameState::addGameObject(GameObject *pGameObj) {
	auto findIt = m_gameObjects.find(pGameObj->getGameObjectID());

	if(findIt != m_gameObjects.cend()){
		printf("Adding %s object with duplicate ids!\n", pGameObj->getObjectType().c_str());
		return false;
	}else{
		m_gameObjects[pGameObj->getGameObjectID()] = pGameObj;
		return true;
	}
};

bool GameState::removeGameObject(GameObjectID objID){
	auto findIt = m_gameObjects.find(objID);
	if(findIt != m_gameObjects.end()){
		delete findIt->second;
		m_gameObjects.erase(findIt);
		return true;
	}else{
		return false;
	}
}