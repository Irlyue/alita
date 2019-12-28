#ifndef ALITA_GAME_OBJECT_FACTORY_H
#define ALITA_GAME_OBJECT_FACTORY_H

#include "GameObject.h"
#include "AlitaStd.h"
#include <map>
#include "MenuButton.h"
#include "Player.h"
#include "Bullet.h"
#include "Level.h"

class GameObjectFactory{
public:

    void init(XMLDocument *doc);

    void add(const ObjectType &objType, GameObjectCreator creator);

    GameObject *create(const ObjectType &objType);

    static GameObjectFactory *getInstance();

private:
    static GameObjectFactory *s_pGameObjectFactory;
    std::map<ObjectType, GameObjectCreator> m_ObjectCreatorMaps;

    XMLDocument *m_pGameObjectInfos;

};

#endif