#ifndef ALITA_GAME_OBJECT_FACTORY_H
#define ALITA_GAME_OBJECT_FACTORY_H

#include "GameObject.h"
#include "AlitaStd.h"
#include <map>
#include "MenuButton.h"
#include "Player.h"
#include "Bullet.h"
#include "Entrance.h"
#include "NPCharacter.h"
#include "Legend.h"

class GameObjectFactory{
public:

	~GameObjectFactory() {destroy();}

	void destroy();

    bool init(XMLElement *doc);

    void add(const ObjectType &objType, GameObjectCreator creator);

    GameObject *create(const ObjectType &objType);

    static GameObjectFactory *getInstance();

    std::map<ObjectType, XMLElement*> &getGameObjectInfos() {return m_gameObjectInfos;}

private:
    static GameObjectFactory *s_pGameObjectFactory;
    std::map<ObjectType, GameObjectCreator> m_ObjectCreatorMaps;

    std::map<ObjectType, XMLElement*> m_gameObjectInfos;

};

#endif