#include "GameObjectFactory.h"

GameObjectFactory *GameObjectFactory::s_pGameObjectFactory = nullptr;

GameObjectFactory *GameObjectFactory::getInstance(){
    if(!s_pGameObjectFactory){
        s_pGameObjectFactory = new GameObjectFactory;
    }

    return s_pGameObjectFactory;
}

bool GameObjectFactory::init(XMLElement *doc){
    m_pGameObjectInfos = doc;

	return true;
}

void GameObjectFactory::add(const ObjectType &objType, GameObjectCreator creator){
    m_ObjectCreatorMaps[objType] = creator;
}


GameObject *GameObjectFactory::create(const ObjectType &objType){
    auto it = m_ObjectCreatorMaps.find(objType);
    if(it != m_ObjectCreatorMaps.cend()){
        GameObject *pGameObject = it->second();
        pGameObject->init(m_pGameObjectInfos->FirstChildElement(objType.c_str()));
        return pGameObject;
    }
    printf("Object type `%s` not found!\n", objType.c_str());
    return nullptr;
}