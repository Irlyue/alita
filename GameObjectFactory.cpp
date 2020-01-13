#include "GameObjectFactory.h"

GameObjectFactory *GameObjectFactory::s_pGameObjectFactory = nullptr;

GameObjectFactory *GameObjectFactory::getInstance(){
    if(!s_pGameObjectFactory){
        s_pGameObjectFactory = GCC_NEW GameObjectFactory;
    }

    return s_pGameObjectFactory;
}

bool GameObjectFactory::init(XMLElement *doc){
    for(auto it = doc->FirstChildElement(); it; it = it->NextSiblingElement()){
        m_gameObjectInfos[it->Value()] = it;
    }

    // for NPC
    for(auto it = doc->FirstChildElement("NPCharacters")->FirstChildElement(); it; it = it->NextSiblingElement()){
        m_gameObjectInfos[it->Attribute("code")] = it;
    }

    // for Monster

    return true;
}

void GameObjectFactory::add(const ObjectType &objType, GameObjectCreator creator){
    m_ObjectCreatorMaps[objType] = creator;
}


GameObject *GameObjectFactory::create(const ObjectType &objType){
    auto it = m_ObjectCreatorMaps.find(objType);
    if(it != m_ObjectCreatorMaps.cend()){
        GameObject *pGameObject = it->second();
        pGameObject->init(m_gameObjectInfos[objType.c_str()]);
        return pGameObject;
    }
    printf("Object type `%s` not found!\n", objType.c_str());
    return nullptr;
}

void GameObjectFactory::destroy(){
    printf("GameObjectFactory destroyed!\n");
}