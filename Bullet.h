#ifndef ALITA_BULLET_H
#define ALITA_BULLET_H

#include "GameObject.h"

class Bullet: public GameObject{
public:

    virtual bool init(const XMLElement *doc);

    virtual void update();

    virtual void draw();
    
protected:

    int m_damage = 0;

};

class PlayerBullet: public Bullet{
public:
    const static ObjectType s_type;

    static GameObject *creator();

    const ObjectType &getObjectType() const {
        return s_type;
    }
};

#endif